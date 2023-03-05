#include "pch.h"
#include "Device.h"
#include "Core/WinWindow.h"



VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		std::cerr << "Validation Layer: " << pCallbackData->pMessageIdName << " : " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

VkResult CreateDebugUtilsMessagerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}

	return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void DestroyDebugUtilsMessagerEXT(VkInstance instance, VkDebugUtilsMessengerEXT pDebugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		func(instance, pDebugMessenger, pAllocator);
	}
}

Device::Device(class WinWindow* window)
	:m_Window(window)
{
	CreateVKInstance();
	SetupDebugMessenger();
	CreateWindowSurface();
	SelectPhysicalDevice();
	CreateLogicalDevice();
	CreateCommandPool();
}

Device::~Device()
{
	vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
	vkDestroyDevice(m_Device, nullptr);

	if (enableValidationLayers)
	{
		DestroyDebugUtilsMessagerEXT(m_VKInstance, m_VKDebugMessenger, nullptr);
	}

	vkDestroySurfaceKHR(m_VKInstance, m_Surface, nullptr);
	vkDestroyInstance(m_VKInstance, nullptr);
}

void Device::CreateVKInstance()
{
	if (enableValidationLayers && !CheckValidationLayerSupport())
	{
		throw std::runtime_error("Validation layers requested, but not avialable!");
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = GetRequiredExtensions();

	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT DebugCreateInfo{};
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(G_ValidationLayers.size());
		createInfo.ppEnabledLayerNames = G_ValidationLayers.data();

		PopulateDebugMessengerCreateInfo(DebugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&DebugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}


	VkResult result = vkCreateInstance(&createInfo, nullptr, &m_VKInstance);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vk instance!");
	}

	CheckVKExtensions();
}

void Device::CheckVKExtensions()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);

	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "Available extensions:\n";
	for (const auto& extension : extensions)
	{
		std::cout << '\t' << extension.extensionName << '\n';
	}

}

void Device::CreateCommandPool()
{
	QueueFamilityIndices queueFamilyIndices = FindQueueFamilies(m_PhysicalDevice);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.m_GraphicsFamily.value();

	if (vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create command pool!");
	}
}

bool Device::CheckValidationLayerSupport() const
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : G_ValidationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}

std::vector<const char*> Device::GetRequiredExtensions() const
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}


void Device::SetupDebugMessenger()
{
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	PopulateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessagerEXT(m_VKInstance, &createInfo, nullptr, &m_VKDebugMessenger) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to set up Debug Messenger");
	}
}

void Device::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = DebugCallback;
	createInfo.pUserData = nullptr;
}

void Device::SelectPhysicalDevice()
{

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_VKInstance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("Failed to find GPUs with vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_VKInstance, &deviceCount, devices.data());

	for (const auto& device : devices)
	{
		if (IsDeviceSuitable(device))
		{
			m_PhysicalDevice = device;
			break;
		}
	}

	if (m_PhysicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to find suitable GPU!");
	}
}

void Device::CreateLogicalDevice()
{
	QueueFamilityIndices indices = FindQueueFamilies(m_PhysicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.m_GraphicsFamily.value(), indices.m_PresentFamily.value() };

	float queuePriority = 1.0f;

	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}


	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();


	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(G_DeviceExtensions.size());
	createInfo.ppEnabledExtensionNames = G_DeviceExtensions.data();

	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(G_ValidationLayers.size());
		createInfo.ppEnabledLayerNames = G_ValidationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create logical device!");
	}

	vkGetDeviceQueue(m_Device, indices.m_GraphicsFamily.value(), 0, &m_GraphicsQueue);
	vkGetDeviceQueue(m_Device, indices.m_PresentFamily.value(), 0, &m_PresentQueue);
}

bool Device::IsDeviceSuitable(VkPhysicalDevice device) const
{
	QueueFamilityIndices indices = FindQueueFamilies(device);

	bool extensionsSupported = CheckDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported)
	{
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.m_Formats.empty() && !swapChainSupport.m_PresentModes.empty();
	}

	return indices.IsComplete() && extensionsSupported && swapChainAdequate;

}

int Device::RateDeviceSuitability(VkPhysicalDevice device) const
{
	int score = 0;

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		score += 1000;
	}

	score += deviceProperties.limits.maxImageDimension2D;

	if (!deviceFeatures.geometryShader) return 0;

	return score;
}

void Device::CreateWindowSurface()
{
	m_Window->CreateWindowSurface(m_VKInstance, &m_Surface);
}

SwapChainSupportDetails Device::QuerySwapChainSupport(VkPhysicalDevice device) const
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &details.m_Capabilities);

	//query formats
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.m_Formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, details.m_Formats.data());
	}

	//query present modes
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.m_PresentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, details.m_PresentModes.data());
	}

	return details;
}

QueueFamilityIndices Device::FindQueueFamilies(VkPhysicalDevice device) const
{
	QueueFamilityIndices indices{};

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, & queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.m_GraphicsFamily = i;
		}

		if (presentSupport)
		{
			indices.m_PresentFamily = i;
		}

		if (indices.IsComplete()) break;

		i++;
	}
	return indices;
}

uint32_t Device::FindMemoryType(uint32_t FilterType, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((FilterType & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type!");
}

bool Device::CheckDeviceExtensionSupport(VkPhysicalDevice device) const
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(G_DeviceExtensions.begin(), G_DeviceExtensions.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}
