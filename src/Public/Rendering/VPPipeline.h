#pragma once

#include <string>
#include <vector>
#include "VPModel.h"
#include "VPEngineDevice.h"
namespace VP {

    struct PipelineConfigInfo {
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };
    class VPPipeline {
    public:
        VPPipeline(VPEngineDevice& device, const std::string & vertFilepath, const std::string & fragFilepath, const PipelineConfigInfo & configData);
        ~VPPipeline();

        VPPipeline(const VPPipeline&) = delete;
        VPPipeline& operator=(const VPPipeline&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        static void GetDefaultPipelineConfigInfo(PipelineConfigInfo &configInfo);

    private:
        static std::vector<char> ReadFile(const std::string& filepath);
        void CreateGraphicsPipeline(const std::string & vertFilepath, const std::string & fragFilepath, const PipelineConfigInfo & configData);

        void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        VPEngineDevice& Device;
        VkPipeline GraphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
};


