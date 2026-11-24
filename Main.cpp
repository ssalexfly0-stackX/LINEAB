//------- Ignore this ----------
#include<filesystem>
#include<cmath>
namespace fs = std::filesystem;
//------------------------------

#include"Model.h"

glm::vec3 bezierPoint
(
	const glm::vec3& p0,
	const glm::vec3& p1,
	const glm::vec3& p2,
	const glm::vec3& p3,
	float t
)
{
	float inverseT = 1.0f - t;
	return inverseT * inverseT * inverseT * p0
		+ 3.0f * inverseT * inverseT * t * p1
		+ 3.0f * inverseT * t * t * p2
		+ t * t * t * p3;
}

glm::vec3 pathPoint(float t)
{
	glm::vec3 p0(-2.0f, -0.5f, -2.0f);
	glm::vec3 p1(-1.0f, 0.1f, -3.0f);
	glm::vec3 p2(1.0f, 1.0f, -3.0f);
	glm::vec3 p3(2.0f, -0.2f, -2.0f);

	return bezierPoint(p0, p1, p2, p3, t);
}


const unsigned int width = 800;
const unsigned int height = 800;


int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);





	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	Shader pathShader("path.vert", "path.frag");

	std::vector<glm::vec3> pathVertices;
	for (unsigned int i = 0; i <= 200; i++)
	{
		float t = static_cast<float>(i) / 200.0f;
		pathVertices.push_back(pathPoint(t));
	}

	GLuint pathVAO;
	GLuint pathVBO;
	glGenVertexArrays(1, &pathVAO);
	glGenBuffers(1, &pathVBO);
	glBindVertexArray(pathVAO);
	glBindBuffer(GL_ARRAY_BUFFER, pathVBO);
	glBufferData(GL_ARRAY_BUFFER, pathVertices.size() * sizeof(glm::vec3), pathVertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(1.0f, 2.0f, 2.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "objectTint"), 1.0f, 0.15f, 0.15f, 1.0f);



	

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 1.8f, 5.0f));
	camera.Orientation = glm::normalize(glm::vec3(0.0f, -0.25f, -1.0f));


	/*
	* I'm doing this relative path thing in order to centralize all the resources into one folder and not
	* duplicate them between tutorial folders. You can just copy paste the resources from the 'Resources'
	* folder and then give a relative path from this folder to whatever resource you want to get to.
	* Also note that this requires C++17, so go to Project Properties, C/C++, Language, and select C++17
	*/
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string modelPath = "/Resources/YoutubeOpenGL 13 - Model Loading/models/bunny/scene.gltf";
	
	// Load in a model
	Model model((parentDir + modelPath).c_str());

	// Original code from the tutorial
	// Model model("models/bunny/scene.gltf");

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// Move the model back and forth along the Bezier path
		float t = 0.5f * (std::sin(static_cast<float>(glfwGetTime()) * 0.35f) + 1.0f);
		glm::vec3 modelPosition = pathPoint(t);

		// Draw the model
		model.Draw
		(
			shaderProgram,
			camera,
			modelPosition,
			glm::vec3(5.0f, 5.0f, 5.0f),
			glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f))
		);

		// Draw the Bezier path
		pathShader.Activate();
		camera.Matrix(pathShader, "camMatrix");
		glUniform4f(glGetUniformLocation(pathShader.ID, "lineColor"), 1.0f, 0.9f, 0.0f, 1.0f);
		glLineWidth(3.0f);
		glDisable(GL_DEPTH_TEST);
		glBindVertexArray(pathVAO);
		glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(pathVertices.size()));
		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	shaderProgram.Delete();
	pathShader.Delete();
	glDeleteBuffers(1, &pathVBO);
	glDeleteVertexArrays(1, &pathVAO);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}