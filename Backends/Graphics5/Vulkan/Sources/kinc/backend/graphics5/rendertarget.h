#pragma once

#include "MiniVulkan.h"

typedef struct {
	VkImage sourceImage;
	VkDeviceMemory sourceMemory;
	VkImageView sourceView;

	VkImage depthImage;
	VkDeviceMemory depthMemory;
	VkImageView depthView;
	int depthBufferBits;

	VkFramebuffer framebuffer;
	VkSampler sampler;

	VkFormat format;

	VkBuffer readbackBuffer;
	VkDeviceMemory readbackMemory;
	bool readbackBufferCreated;

	int stage;
	int stage_depth;
} RenderTarget5Impl;
