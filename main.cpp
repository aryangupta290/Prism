#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <bits/stdc++.h>
#include "camera.h"
#include "shader.h"
using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void query_solver(GLFWwindow *window, glm::mat4 &transform);
void key_was_pressed(GLFWwindow *window, int key, int scancode, int action, int mods);
glm::vec3 centre = glm::vec3(0.0f, 0.0f, 0.0f);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
// glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
// glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
// glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//  timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;
// Camera camera(glm:vikrant.dewangan@research.iiit.ac.in:vec3(0.0f, 0.0f, 10.0f));

glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 prism_center = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 direction_vector = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 up_vector = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 original = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 00.0f);
int rot = 0;
int revolve = 0;
bool is_translate = false;
int main(int argc, char *argv[])
{
	srand(time(0));
	if (argc < 2)
	{
		std::cout << "ERROR : Incorrect number of arguments." << std::endl;
		return -1;
	}
	int n = atoi(argv[1]);
	// int n;
	// printf("Enter number of vertices : ");
	// std::cin >> n;
	//  glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	GLFWwindow *window =
		glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	Shader ourShader("../src/vertex.shader", "../src/fragment.shader");

	// float vertices[18 * n];
	float r = 0.4;
	float A[n], B[n];
	double angle = 360.00 / n;
	double now = 0;
	for (int i = 0; i < n; i++)
	{
		double rad = (now * 3.14159) / 180;
		double res = r * cos(rad);
		A[i] = res; // x cco
		res = r * sin(rad);
		B[i] = res;
		now += angle;
	}
	float C[n][3];
	for (int i = 0; i < n; i++)
	{
		C[i][0] = A[i];
		C[i][1] = B[i];
		C[i][2] = -0.5;
	}
	float D[n][3];
	for (int i = 0; i < n; i++)
	{
		D[i][0] = A[i];
		D[i][1] = B[i];
		D[i][2] = 0.5;
	}
	float vertices[72 * n];
	float col[3] = {1.0f, 0.5f, 0.2f};
	float temp[] = {0.0f, 0.0f, -0.5f};
	int a = 0;
	for (int i = 0; i < 18 * n; i += 18)
	{
		for (int j = i; j < i + 3; j++)
		{
			vertices[j] = C[a][j - i];
		}
		for (int j = i + 3; j < i + 6; j++)
		{
			vertices[j] = col[j - i - 3];
		}
		for (int j = i + 6; j < i + 9; j++)
		{
			vertices[j] = temp[j - i - 6];
		}
		for (int j = i + 9; j < i + 12; j++)
		{
			vertices[j] = col[j - i - 9];
		}
		for (int j = i + 12; j < i + 15; j++)
		{
			vertices[j] = C[(a + 1) % n][j - i - 12];
		}
		for (int j = i + 15; j < i + 18; j++)
		{
			vertices[j] = col[j - i - 15];
		}
		a++;
	}
	temp[2] = 0.5f;
	a = 0;
	for (int i = 18 * n; i < 36 * n; i += 18)
	{
		//	int a = 0;
		for (int j = i; j < i + 3; j++)
		{
			vertices[j] = D[a][j - i];
		}
		for (int j = i + 3; j < i + 6; j++)
		{
			vertices[j] = col[j - i - 3];
		}
		for (int j = i + 6; j < i + 9; j++)
		{
			vertices[j] = temp[j - i - 6];
		}
		for (int j = i + 9; j < i + 12; j++)
		{
			vertices[j] = col[j - i - 9];
		}
		for (int j = i + 12; j < i + 15; j++)
		{
			vertices[j] = D[(a + 1) % n][j - i - 12];
		}
		for (int j = i + 15; j < i + 18; j++)
		{
			vertices[j] = col[j - i - 15];
		}
		a++;
	}
	a = 0;
	for (int i = 36 * n; i < 72 * n;)
	{
		// 2 traingles
		// so 36 vertices
		// first triangle
		float t = (float)rand() / (RAND_MAX);
		float col[3];
		col[0] = t;
		t = (float)rand() / (RAND_MAX);
		col[1] = t;
		t = (float)rand() / (RAND_MAX);
		col[2] = t;
		int b = 0;
		int c = i;
		for (; i < c + 3; i++)
		{
			vertices[i] = C[a][b];
			b++;
		}
		c = i;

		b = 0;
		for (; i < c + 3; i++)
		{
			vertices[i] = col[b];
			b++;
		}
		c = i;
		b = 0;
		for (; i < c + 3; i++)
		{
			vertices[i] = C[(a + 1) % n][b];
			b++;
		}
		c = i;
		b = 0;
		for (; i < c + 3; i++)
		{
			vertices[i] = col[b];
			b++;
		}
		c = i;
		b = 0;
		for (; i < c + 3; i++)
		{
			vertices[i] = D[a][b];
			b++;
		}
		c = i;
		b = 0;
		for (; i < c + 3; i++)
		{
			vertices[i] = col[b];
			b++;
		}
		c = i;
		b = 0;
		for (; i < c + 3; i++)
		{
			vertices[i] = D[a][b];
			b++;
		}
		c = i;

		b = 0;
		for (; i < c + 3; i++)
		{
			vertices[i] = col[b];
			b++;
		}
		c = i;
		b = 0;
		for (; i < c + 3; i++)
		{
			vertices[i] = D[(a + 1) % n][b];
			b++;
		}
		c = i;
		b = 0;
		for (; i < c + 3; i++)
		{
			vertices[i] = col[b];
			b++;
		}
		c = i;
		b = 0;
		for (; i < c + 3; i++)
		{
			vertices[i] = C[(a + 1) % n][b];
			b++;
		}
		c = i;
		b = 0;
		for (; i < c + 3; i++)
		{
			vertices[i] = col[b];
			b++;
		}
		a++;
		// cout << a << endl;
	}
	// now for the remaining vetices , two triangle in each
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// float vertices[] = {
	// 	// first triangle
	// 	r, 0, -0.5f,		// left
	// 	-0.0f, -0.5f, 0.0f, // right
	// 	-0.45f, 0.5f, 0.0f, // top
	// 						// second triangle
	// 	0.0f, -0.5f, 0.0f,	// left
	// 	0.9f, -0.5f, 0.0f,	// right
	// 	0.45f, 0.5f, 0.0f	// top
	// };

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	//	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// glm::mat4 projection = glm::mat4(1.0f); // identity matrix
	// ourShader.setMat4("projection", projection);
	// render loop

	// how to bring back everything to original position on changing anything
	glm::mat4 transform = glm::mat4(1.0f);
	glfwSetKeyCallback(window, key_was_pressed);
	glm::mat4 view = glm::lookAt(camera_position, prism_center, up_vector);
	angle = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);
		query_solver(window, transform);
		if (rot)
		{
			angle += 0.1f;
			// glm::mat4 tempMat = glm::mat4(1.0f);
			// tempMat = glm::translate(tempMat, prism_center);
			// tempMat = glm::rotate(tempMat, (float)angle, glm::vec3(1.0f, 0.0f, 0.0f));
		}	// tempMat = glm::translate(transform, -prism_center);

			transform = glm::translate(glm::mat4(1.0f), prism_center);
			transform = glm::rotate(transform, (float)angle, glm::vec3(1.0f, 0.0f, 0.0f));

		if (revolve)
		{

			// std::cout << camera_position.x << " " << camera_position.y << " " << camera_position.z << std::endl;
			// glm::vec3 right_vector = glm::normalize(glm::cross(prism_center - camera_position, world_up));
			// right_vector *= .05;

			// //	std::cout << dir.x << " " << dir.y << " " << dir.z << std::endl;

			// glm::mat4 trans = glm::mat4(1.0f);
			// trans = glm::translate(trans, right_vector);
			// glm::vec4 temp = glm::vec4(camera_position, 1.0f);
			// temp = (trans * temp);
			// camera_position = glm::vec3(temp.x, temp.y, temp.z);

			camera_position -= prism_center;
			glm::mat4 temp = glm::mat4(1.0f);
			temp = glm::rotate(temp, 0.05f, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec4 temp2 = temp * glm::vec4(camera_position, 1.0f);
			camera_position = glm::vec3(temp2);
			camera_position += prism_center;

			// glm::vec3 right_vector = glm::normalize(glm::cross(prism_center - camera_position, world_up));
			// up_vector = glm::normalize(glm::cross(right_vector, prism_center - camera_position));
			// // cout << up_vector.x << " " << up_vector.y << " " << up_vector.z << endl;
			// glm::vec3 temp = glm::normalize(glm::cross(prism_center - camera_position, up_vector)) * 0.2f;
			// //cout << temp.x << " " << temp.y << " " << temp.z << endl;
			// camera_position += temp;
			// //	cout << camera_position.x << " " << camera_position.y << " " << camera_position.z << endl;
		}

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// make transformations

		// transform = glm::rotate(transform, (float)glfwGetTime(),
		// 						glm::vec3(1.0f, 1.0f, 0.0f));
		// ourShader.setMat4("transform", transform);
		//  camera/view transformation
		if (!is_translate)
		{
			glm::vec3 right_vector = glm::normalize(glm::cross(prism_center - camera_position, world_up));
			view = glm::lookAt(camera_position, prism_center, world_up);
			// cout << prism_center.x << " " << prism_center.y << " " << prism_center.z << endl;
		}
		//  ourShader.setMat4("view", view);
		unsigned int transformLoc =
			glGetUniformLocation(ourShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
						   glm::value_ptr(transform));
		unsigned int viewLoc =
			glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE,
						   glm::value_ptr(view));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);
		ourShader.use();
		// processInput(window);

		glBindVertexArray(VAO); // seeing as we only have a single VAO there's
		// no need to bind it every time, but we'll do
		// so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 72 * n);
		// glBindVertexArray(0); // no need to unbind it every time

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse
		// moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
		//	std::cout << is_translate << endl;
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	//	glDeleteVertexArrays(1, &VAO);
	//	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
void processInput(GLFWwindow *window)
{
	const float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
void query_solver(GLFWwindow *window, glm::mat4 &transform)
{
	const float cameraSpeed = 0.05f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		is_translate = false;

		camera_position -= cameraSpeed * glm::vec3(0.0f, 0.0f, 1.0f);
		//	std::cout << camera_position.z << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		is_translate = false;
		camera_position -= cameraSpeed * glm::vec3(0.0f, 0.0f, -1.0f);
		//	std::cout << camera_position.z << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		is_translate = false;

		camera_position += cameraSpeed * glm::vec3(-1.0f, 0.0f, 0.0f);
		//	std::cout << camera_position.z << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		is_translate = false;

		camera_position += cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
		//	std::cout << camera_position.z << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		is_translate = false;
		camera_position += cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
		//	std::cout << camera_position.z << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		is_translate = false;
		camera_position += cameraSpeed * glm::vec3(0.0f, -1.0f, 0.0f);
		//	std::cout << camera_position.z << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		is_translate = false;
		camera_position = glm::vec3(1.0f, 0.5f, 7.25f);
		//	std::cout << camera_position.z << endl;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		is_translate = false;

		camera_position = glm::vec3(0.5f, 1.0f, 12.5f);
		//	std::cout << camera_position.z << endl;
	}
	// towards

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		if (!revolve)
			is_translate = true;
		glm::vec3 temp = glm::normalize(camera_position - prism_center) * cameraSpeed;
		prism_center += temp;
		// transform = glm::translate(transform, glm::normalize(camera_position - prism_center) * cameraSpeed);
	}
	// away
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		if (!revolve)
			is_translate = true;
		glm::vec3 temp = glm::normalize(camera_position - prism_center) * cameraSpeed;
		prism_center -= temp;
		// transform = glm::translate(transform, glm::normalize(-camera_position + prism_center) * cameraSpeed);
	}

	// up

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		if (!revolve)
			is_translate = true;
		glm::vec3 right_vector = glm::normalize(glm::cross(prism_center - camera_position, world_up));
		up_vector = glm::normalize(glm::cross(right_vector, prism_center - camera_position));

		prism_center += up_vector * cameraSpeed;
		//	cout << prism_center.x << " " << prism_center.y << " " << prism_center.z << endl;
		// transform = glm::translate(transform, up_vector * cameraSpeed);
	}

	// down

	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
	{
		if (!revolve)
			is_translate = true;
		glm::vec3 right_vector = glm::normalize(glm::cross(prism_center - camera_position, world_up));
		up_vector = glm::normalize(glm::cross(right_vector, prism_center - camera_position));

		prism_center -= up_vector * cameraSpeed;
		// transform = glm::translate(transform, -up_vector * cameraSpeed);
	}

	// right

	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
	{
		if (!revolve)
			is_translate = true;
		glm::vec3 right_vector = glm::normalize(glm::cross(prism_center - camera_position, world_up));
		up_vector = glm::normalize(glm::cross(right_vector, prism_center - camera_position));
		prism_center += right_vector * cameraSpeed;
		// transform = glm::translate(transform, right_vector * cameraSpeed);
	}
	// left

	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
	{
		if (!revolve)
			is_translate = true;
		glm::vec3 right_vector = glm::normalize(glm::cross(prism_center - camera_position, world_up));
		up_vector = glm::normalize(glm::cross(right_vector, prism_center - camera_position));
		prism_center -= right_vector * cameraSpeed;
		// transform = glm::translate(transform, -right_vector * cameraSpeed);
	}
}
void key_was_pressed(GLFWwindow *window, int key, int scancode, int action, int mods)
{

	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		// is_translate = false;
		rot = rot ^ 1;
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		//	prism_center = glm::vec3(0.0f, 0.0f, 0.0f);
		is_translate = false;
		revolve = revolve ^ 1;
	}
}
// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width
	// and height will be significantly larger than specified on retina
	// displays.
	glViewport(0, 0, width, height);
}
