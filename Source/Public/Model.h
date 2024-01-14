#pragma once

#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <GLFW/glfw3.h>

#include "Device.h"
#include "SwapChain.h"

#include <glm/glm.hpp>
#include <vector>
#include <array>

namespace Application
{
	struct UniformBufferObject
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 color;

		static VkVertexInputBindingDescription getBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			// Note : We need to specifies the number of bytes from one entry to the next
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
		{
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

			// Position attribute
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			// Color attribute
			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			// Note : the color attribute is at an offset in the vertex struct because it's the 2nd parameter
			attributeDescriptions[1].offset = offsetof(Vertex, color); 

			return attributeDescriptions;
		}
	};

	class Model
	{
	public:
		Model(Device device, SwapChain swapChain);
		void Cleanup();

		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
			VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		VkBuffer GetVertexBuffer() { return vertexBuffer; }
		VkBuffer GetIndexBuffer() { return indexBuffer; }
		VkDescriptorSetLayout& GetDescriptorLayout() { return descriptorSetLayout; }
		VkDescriptorSet& GetDescriptorSets(int32_t index) { return descriptorSets[index]; }

		const std::vector<Vertex> vertices =
		{
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};

		// If we use more than 65535 unique vertices, we need to up the uint16_t to uint32_t.
		const std::vector<uint16_t> indices =
		{
			0,1,2,2,3,0
		};

		void UpdateUniformBuffer(uint32_t currentImage);
	private:
		void CreateVertexBuffers();
		void CreateIndexBuffers();
		void CreateUniformBuffers();
		void CreateDescriptionSetLayout();
		void CreateDescriptorPool();
		void CreateDescriptorSet();


		Device device;
		SwapChain swapChain;

		// Buffers.
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		// The uniform buffer will be calculate every frame
		// but we don't want the next frame to read the current uniform buffer
		// so each frame in flight will have it's on uniform buffer.
		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		std::vector<void*> uniformBuffersMapped;

		// Descriptors
		VkDescriptorSetLayout descriptorSetLayout;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSet> descriptorSets;
						
	};

}
