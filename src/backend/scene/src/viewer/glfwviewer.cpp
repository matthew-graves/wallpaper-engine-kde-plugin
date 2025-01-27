#include <iostream>
#include <set>
#include <fstream>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <atomic>
#include "pkg.h"
#include "wallpaper.h"

#include <unistd.h>

using namespace std;

unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 720;
atomic<bool> renderCall(false);
wallpaper::WallpaperGL* pwgl = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if(pwgl)
		pwgl->SetMousePos(xpos, ypos);
}

void updateCallback() {
	renderCall = true;
	glfwPostEmptyEvent();
}

int main(int argc, char**argv)
{
	int result = 0,objnum = -1,effnum = -1;
    while((result = getopt(argc, argv, "o:e:")) != -1 ) {
		switch(result)
		{
		case 'o':
			objnum = std::stoi(optarg);		
			break;
		case 'e':
			effnum = std::stoi(optarg);
			break;
			break;
		case '?':
			break;
		}
    }
	if(argc - optind != 2) {
		std::cerr << "usage: "+ std::string(argv[0]) +" [options] <assets dir> <pkg file>\n"
		<< "options:\n"  
		<< "-o object amount to render\n" 
		<< "-e effect amount to render\n";
		return 1;
	}

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "WP", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    pwgl = new wallpaper::WallpaperGL();
    auto& wgl = *pwgl;
	wgl.Init((GLADloadproc)glfwGetProcAddress);
	wgl.SetAssets(argv[optind]);
	wgl.SetObjEffNum(objnum, effnum);
	wgl.SetFlip(true);
    //const wallpaper::fs::file_node& x = wallpaper::WallpaperGL::GetPkgfs();
    wgl.Load(argv[optind+1]);

	wgl.SetUpdateCallback(updateCallback);

	// glfw callback
	glfwSetCursorPosCallback(window, cursor_position_callback);

    while (!glfwWindowShouldClose(window)) {
		glfwWaitEvents();
		if(renderCall) {
			renderCall = false;
			wgl.Render(0,SCR_WIDTH,SCR_HEIGHT);
			glfwSwapBuffers(window);
		}
    }
    delete pwgl;
    //wgl.Clear();
    glfwTerminate();
    return 0;
}
