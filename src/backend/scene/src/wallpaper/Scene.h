#pragma once
#include <unordered_map>

#include "SceneTexture.h"
#include "SceneRenderTarget.h"
#include "SceneNode.h"
#include "Interface/IShaderValueUpdater.h"
#include "Interface/IImageParser.h"


namespace wallpaper
{

class Scene {
public:
	std::unordered_map<std::string, std::shared_ptr<SceneTexture>> textures;
	std::unordered_map<std::string, SceneRenderTarget> renderTargets;
	std::unordered_map<std::string, SceneBindRenderTarget> renderTargetBindMap;

	std::unordered_map<std::string, std::shared_ptr<SceneCamera>> cameras;
	std::unordered_map<std::string, std::vector<std::string>> linkedCameras;

	std::shared_ptr<SceneNode> sceneGraph;	
	std::unique_ptr<IShaderValueUpdater> shaderValueUpdater;
	std::unique_ptr<IImageParser> imageParser;

	SceneCamera* activeCamera;

	double elapsingTime {0.0f};
	std::vector<float> clearColor {1.0f, 1.0f, 1.0f};

	void UpdateLinkedCamera(const std::string& name) {
		if(linkedCameras.count(name) != 0) {
			auto& cams = linkedCameras.at(name);
			for(auto& cam:cams) {
				if(cameras.count(cam) != 0) {
					cameras.at(cam)->Clone(*cameras.at(name));
				}
			}
		}
	}
};
}
