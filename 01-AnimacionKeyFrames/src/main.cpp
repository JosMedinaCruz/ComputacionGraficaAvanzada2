#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

#include "Headers/AnimationUtils.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para multiples texturas
Shader shaderMultiplesTexturas;

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere skyboxSphere(20, 20);
Box boxCesped;
Box boxWalls;
Box boxHighway;
Box boxLandingPad;
Sphere esfera1(20, 20, 1.0);
//Cubo con multiples texturas
Box boxMultTexturas;

// Models complex instances
Model modelRock;
Model modelAircraft;
Model modelEclipseChasis;
Model modelEclipseRearWheels;
Model modelEclipseFrontalWheels;
//Model modelHeliChasis;
//Model modelHeliHeli;
Model modelLambo;
Model modelLamboLeftDor;
Model modelLamboRightDor;
Model modelLamboFrontLeftWheel;
Model modelLamboFrontRightWheel;
Model modelLamboRearLeftWheel;
Model modelLamboRearRightWheel;
// Dart lego
Model modelDartLegoBody;
Model modelDartLegoHead;
Model modelDartLegoMask;
Model modelDartLegoLeftArm;
Model modelDartLegoRightArm;
Model modelDartLegoLeftHand;
Model modelDartLegoRightHand;
Model modelDartLegoLeftLeg;
Model modelDartLegoRightLeg;
//Buzz
Model modelBuzzHead;
Model modelBuzzTorso;
Model modelBuzzLeftArm;
Model modelBuzzLeftForeArm;
Model modelBuzzLeftHand;
//**************************
Model modelBuzzHip;
Model modelBuzzRightArm;
Model modelBuzzRightForeArm;
Model modelBuzzRightHand;
Model modelBuzzLeftWing1;
Model modelBuzzLeftWing2;
Model modelBuzzRightWing1;
Model modelBuzzRightWing2;
Model modelBuzzLeftCalf;
Model modelBuzzLeftFoot;
Model modelBuzzLeftThigh;
Model modelBuzzRightCalf;
Model modelBuzzRightFoot;
Model modelBuzzRightThigh;

//Helicopter
Model modelHeliChasis;
Model modelHeliHeli;
Model modelMiniHeli;

//Hazmat
Model modelHazmat;
// Rose
Model modelRose;
GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID; //pista de aterrizaje, ultimo
GLuint skyboxTextureID;
GLuint textureWaterID, textureSpongeID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga",
		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
		"../Textures/mp_bloodvalley/blood-valley_up.tga",
		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
glm::mat4 modelMatrixEclipse = glm::mat4(1.0f);
glm::mat4 matrixModelRock = glm::mat4(1.0);
glm::mat4 modelMatrixHeli = glm::mat4(1.0f);
glm::mat4 modelMatrixLambo = glm::mat4(1.0);
glm::mat4 modelMatrixAircraft = glm::mat4(1.0);
glm::mat4 modelMatrixDart = glm::mat4(1.0f);
glm::mat4 modelMatrixBuzz = glm::mat4(1.0f);
//hazmat
glm::mat4 modelMatrixHazmat = glm::mat4(1.0f);
// Rose
glm::mat4 modelMatrixRose = glm::mat4(1.0f);

float rotDartHead = 0.0, rotDartLeftArm = 0.0, rotDartLeftHand = 0.0, rotDartRightArm = 0.0, rotDartRightHand = 0.0, rotDartLeftLeg = 0.0, rotDartRightLeg = 0.0;
float rotBuzzHead = 0.0, rotBuzzLeftArm = 0.0, rotBuzzLeftHand = 0.0, rotBuzzRightArm = 0.0, rotBuzzRightHand = 0.0, rotBuzzLeftThigh = 0.0, rotBuzzRightThigh = 0.0;
int modelSelected = 0;
bool enableCountSelected = true;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;

// Joints interpolations Dart Lego
std::vector<std::vector<float>> keyFramesDartJoints;
std::vector<std::vector<glm::mat4>> keyFramesDart;
int indexFrameDartJoints = 0;
int indexFrameDartJointsNext = 1;
float interpolationDartJoints = 0.0;
int maxNumPasosDartJoints = 20;
int numPasosDartJoints = 0;
int indexFrameDart = 0;
int indexFrameDartNext = 1;
float interpolationDart = 0.0;
int maxNumPasosDart = 200;
int numPasosDart = 0;

// Joints interpolations Buzz
std::vector<std::vector<float>> keyFramesBuzzJoints;
std::vector<std::vector<glm::mat4>> keyFramesBuzz;
int indexFrameBuzzJoints = 0;
int indexFrameBuzzJointsNext = 1;
float interpolationBuzzJoints = 0.0;
int maxNumPasosBuzzJoints = 20;
int numPasosBuzzJoints = 0;
int indexFrameBuzz = 0;
int indexFrameBuzzNext = 1;
float interpolationBuzz = 0.0;
int maxNumPasosBuzz = 200;
int numPasosBuzz = 0;

// Var animate helicopter
float rotHelHelY = 0.0;
float rotHelHelX = 0.0;
int heliState = 0.0;
int rotHelix = 0.0;

//Var move helicopter
float movHeliX = 0.0;
float movHeliY = 0.0;
float movHeliZ = 0.0;
float stopHeli = 0.0;

// Var animate lambo door
int stateDoor = 0;
float dorRotCount = 0.0;

double deltaTime;
double currTime, lastTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
	int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;
	//version de gl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback); //evento teclado
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_texture_res.vs", "../Shaders/multipleLights.fs");
	shaderMultiplesTexturas.initialize("../Shaders/iluminacion_texture_res.vs", "../Shaders/multipleLightsEjercicio.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	esfera1.init();
	esfera1.setShader(&shaderMulLighting);
	esfera1.setPosition(glm::vec3(0.0f, 2.0f, -5.0f));

	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	boxWalls.init();
	boxWalls.setShader(&shaderMulLighting);

	boxHighway.init();
	boxHighway.setShader(&shaderMulLighting);

	boxLandingPad.init();
	boxLandingPad.setShader(&shaderMulLighting);

	boxMultTexturas.init();
	boxMultTexturas.setShader(&shaderMultiplesTexturas);

	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);

	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);

	// Eclipse
	modelEclipseChasis.loadModel("../models/Eclipse/2003eclipse_chasis.obj");
	modelEclipseChasis.setShader(&shaderMulLighting);
	modelEclipseFrontalWheels.loadModel("../models/Eclipse/2003eclipse_frontal_wheels.obj");
	modelEclipseFrontalWheels.setShader(&shaderMulLighting);
	modelEclipseRearWheels.loadModel("../models/Eclipse/2003eclipse_rear_wheels.obj");
	modelEclipseRearWheels.setShader(&shaderMulLighting);
	// Helicopter
	modelHeliChasis.loadModel("../models/Helicopter/HelicopteroChasis.obj");
	modelHeliChasis.setShader(&shaderMulLighting);
	modelHeliHeli.loadModel("../models/Helicopter/HelicopteroHelice1.obj");
	modelHeliHeli.setShader(&shaderMulLighting);
	modelMiniHeli.loadModel("../models/Helicopter/HelicopteroHelice2.obj");
	modelMiniHeli.setShader(&shaderMulLighting);
	// Lamborginhi
	modelLambo.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_chasis.obj");
	modelLambo.setShader(&shaderMulLighting);
	modelLamboLeftDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_left_dor.obj");
	modelLamboLeftDor.setShader(&shaderMulLighting);
	modelLamboRightDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_right_dor.obj");
	modelLamboRightDor.setShader(&shaderMulLighting);
	modelLamboFrontLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_left_wheel.obj");
	modelLamboFrontLeftWheel.setShader(&shaderMulLighting);
	modelLamboFrontRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_right_wheel.obj");
	modelLamboFrontRightWheel.setShader(&shaderMulLighting);
	modelLamboRearLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_left_wheel.obj");
	modelLamboRearLeftWheel.setShader(&shaderMulLighting);
	modelLamboRearRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_right_wheel.obj");
	modelLamboRearRightWheel.setShader(&shaderMulLighting);

	// Dart Lego
	modelDartLegoBody.loadModel("../models/LegoDart/LeoDart_body.obj");
	modelDartLegoBody.setShader(&shaderMulLighting);
	modelDartLegoMask.loadModel("../models/LegoDart/LeoDart_mask.obj");
	modelDartLegoMask.setShader(&shaderMulLighting);
	modelDartLegoHead.loadModel("../models/LegoDart/LeoDart_head.obj");
	modelDartLegoHead.setShader(&shaderMulLighting);
	modelDartLegoLeftArm.loadModel("../models/LegoDart/LeoDart_left_arm.obj");
	modelDartLegoLeftArm.setShader(&shaderMulLighting);
	modelDartLegoRightArm.loadModel("../models/LegoDart/LeoDart_right_arm.obj");
	modelDartLegoRightArm.setShader(&shaderMulLighting);
	modelDartLegoLeftHand.loadModel("../models/LegoDart/LeoDart_left_hand.obj");
	modelDartLegoLeftHand.setShader(&shaderMulLighting);
	modelDartLegoRightHand.loadModel("../models/LegoDart/LeoDart_right_hand.obj");
	modelDartLegoRightHand.setShader(&shaderMulLighting);
	modelDartLegoLeftLeg.loadModel("../models/LegoDart/LeoDart_left_leg.obj");
	modelDartLegoLeftLeg.setShader(&shaderMulLighting);
	modelDartLegoRightLeg.loadModel("../models/LegoDart/LeoDart_right_leg.obj");
	modelDartLegoRightLeg.setShader(&shaderMulLighting);
	//Buzz
	modelBuzzHead.loadModel("../models/buzz/buzzlightyHead.obj");
	modelBuzzHead.setShader(&shaderMulLighting);
	modelBuzzTorso.loadModel("../models/buzz/buzzlightyTorso.obj");
	modelBuzzTorso.setShader(&shaderMulLighting);
	modelBuzzLeftArm.loadModel("../models/buzz/buzzlightyLeftArm.obj");
	modelBuzzLeftArm.setShader(&shaderMulLighting);
	modelBuzzLeftForeArm.loadModel("../models/buzz/buzzlightyLeftForearm.obj");
	modelBuzzLeftForeArm.setShader(&shaderMulLighting);
	modelBuzzLeftHand.loadModel("../models/buzz/buzzlightyLeftHand.obj");
	modelBuzzLeftHand.setShader(&shaderMulLighting);
	//****************************************************
	modelBuzzHip.loadModel("../models/buzz/buzzlightyHip.obj");
	modelBuzzHip.setShader(&shaderMulLighting);
	modelBuzzRightArm.loadModel("../models/buzz/buzzlightyRightArm.obj");
	modelBuzzRightArm.setShader(&shaderMulLighting);
	modelBuzzRightForeArm.loadModel("../models/buzz/buzzlightyRightForearm.obj");
	modelBuzzRightForeArm.setShader(&shaderMulLighting);
	modelBuzzRightHand.loadModel("../models/buzz/buzzlightyRightHand.obj");
	modelBuzzRightHand.setShader(&shaderMulLighting);
	modelBuzzLeftWing1.loadModel("../models/buzz/buzzlightyLeftWing1.obj");
	modelBuzzLeftWing1.setShader(&shaderMulLighting);
	modelBuzzLeftWing2.loadModel("../models/buzz/buzzlightyLeftWing2.obj");
	modelBuzzLeftWing2.setShader(&shaderMulLighting);
	modelBuzzRightWing1.loadModel("../models/buzz/buzzlightyRightWing1.obj");
	modelBuzzRightWing1.setShader(&shaderMulLighting);
	modelBuzzRightWing2.loadModel("../models/buzz/buzzlightyRightWing2.obj");
	modelBuzzRightWing2.setShader(&shaderMulLighting);
	modelBuzzLeftCalf.loadModel("../models/buzz/buzzlightyLeftCalf.obj");
	modelBuzzLeftCalf.setShader(&shaderMulLighting);
	modelBuzzLeftFoot.loadModel("../models/buzz/buzzlightyLeftFoot.obj");
	modelBuzzLeftFoot.setShader(&shaderMulLighting);
	modelBuzzLeftThigh.loadModel("../models/buzz/buzzlightyLeftThigh.obj");
	modelBuzzLeftThigh.setShader(&shaderMulLighting);
	modelBuzzRightCalf.loadModel("../models/buzz/buzzlightyRightCalf.obj");
	modelBuzzRightCalf.setShader(&shaderMulLighting);
	modelBuzzRightFoot.loadModel("../models/buzz/buzzlightyRightFoot.obj");
	modelBuzzRightFoot.setShader(&shaderMulLighting);
	modelBuzzRightThigh.loadModel("../models/buzz/buzzlightyRightThigh.obj");
	modelBuzzRightThigh.setShader(&shaderMulLighting);
	//*******************************************************************

	//hazmat
	modelHazmat.loadModel("../models/hazmat/hazmat.obj");
	modelHazmat.setShader(&shaderMulLighting);
	// Rose
	modelRose.loadModel("../models/Rose/rose.obj");
	modelRose.setShader(&shaderMulLighting);


	camera->setPosition(glm::vec3(0.0, 3.0, 4.0));

	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	FIBITMAP *bitmap;
	unsigned char *data;

	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
			imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
				GL_BGRA, GL_UNSIGNED_BYTE, data);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

	// Definiendo la textura a utilizar
	Texture textureCesped("../Textures/cesped.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureCesped.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureCesped.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureCespedID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCesped.freeImage(bitmap);
	
	//*******************************Ejercicio multiples texturas********************************************
	// Definiendo la textura a utilizar
	Texture textureWater("../Textures/water.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureWater.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureWater.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureWaterID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWaterID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWater.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureSponge("../Textures/sponge.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureSponge.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureSponge.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureSpongeID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureSpongeID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureSponge.freeImage(bitmap);



	// Definiendo la textura a utilizar
	Texture textureWall("../Textures/whiteWall.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureWall.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureWall.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureWallID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWallID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWall.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureWindow("../Textures/ventana.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureWindow.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureWindow.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureWindowID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWindowID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWindow.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureHighway("../Textures/highway.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureHighway.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureHighway.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureHighwayID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureHighwayID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureHighway.freeImage(bitmap);

	//Definir la textura utilizar
	Texture textureLandingPad("../Textures/landingPad.jpg");
	//Carga el mapa de bits
	bitmap = textureLandingPad.loadImage();
	//Convertir el arreglo de bits a un arreglo unidimensional
	data = textureLandingPad.convertToData(bitmap, imageWidth, imageHeight);
	//Generar la textura
	glGenTextures(1, &textureLandingPadID);
	//Enlazar textura a utilizar
	glBindTexture(GL_TEXTURE_2D, textureLandingPadID);
	//Configuración del wraping de la textura
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Configuracion del filtering     Linear interpolación linear
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		//Transferir los datos a memoria
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		//Generar los niveles del mipmap
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Falla al cargar la textura" << std::endl;
		//Liberar memoria de la textura
		textureLandingPad.freeImage(bitmap);
	}
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCesped.destroy();
	boxWalls.destroy();
	boxHighway.destroy();
	boxLandingPad.destroy();
	boxMultTexturas.destroy();

	// Custom objects Delete
	modelAircraft.destroy();
	modelDartLegoBody.destroy();
	modelDartLegoHead.destroy();
	modelDartLegoLeftArm.destroy();
	modelDartLegoLeftHand.destroy();
	modelDartLegoLeftLeg.destroy();
	modelDartLegoMask.destroy();
	modelDartLegoRightArm.destroy();
	modelDartLegoRightHand.destroy();
	modelDartLegoRightLeg.destroy();
	modelEclipseChasis.destroy();
	modelEclipseFrontalWheels.destroy();
	modelEclipseRearWheels.destroy();
	modelHeliChasis.destroy();
	modelHeliHeli.destroy();
	modelMiniHeli.destroy();
	modelLambo.destroy();
	modelLamboFrontLeftWheel.destroy();
	modelLamboFrontRightWheel.destroy();
	modelLamboLeftDor.destroy();
	modelLamboRearLeftWheel.destroy();
	modelLamboRearRightWheel.destroy();
	modelLamboRightDor.destroy();
	modelRock.destroy();
	// Buzz Destroy
	modelBuzzHead.destroy();
	modelBuzzLeftArm.destroy();
	modelBuzzLeftForeArm.destroy();
	modelBuzzLeftHand.destroy();
	modelBuzzTorso.destroy();

	modelBuzzHip.destroy();
	modelBuzzRightArm.destroy();
	modelBuzzRightForeArm.destroy();
	modelBuzzRightHand.destroy();
	modelBuzzLeftWing1.destroy();
	modelBuzzLeftWing2.destroy();
	modelBuzzRightWing1.destroy();
	modelBuzzRightWing2.destroy();
	modelBuzzLeftCalf.destroy();
	modelBuzzLeftFoot.destroy();
	modelBuzzLeftThigh.destroy();
	modelBuzzRightCalf.destroy();
	modelBuzzRightFoot.destroy();
	modelBuzzRightThigh.destroy();
	//hazmat
	modelHazmat.destroy();
	//Rose
	modelRose.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureWallID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
	int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
	offsetX = 0;
	offsetY = 0;

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		enableCountSelected = false;
		modelSelected++;
		if (modelSelected > 5)
			modelSelected = 0;
		if (modelSelected == 1)
			fileName = "../animaciones/animation_dart_joints.txt";
		if (modelSelected == 2)
			fileName = "../animaciones/animation_dart.txt";
		if (modelSelected == 3)
			fileName = "../animaciones/animation_buzz_joints.txt";
		if (modelSelected == 4)
			fileName = "../animaciones/animation_buzz.txt";
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;

	// Guardar key frames
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
		&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		record = true;
		if (myfile.is_open())
			myfile.close();
		myfile.open(fileName);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE
		&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		record = false;
		myfile.close();
		if (modelSelected == 1)
			keyFramesDartJoints = getKeyRotFrames(fileName);
		if (modelSelected == 2)
			keyFramesDart = getKeyFrames(fileName);
		if (modelSelected == 3)
			keyFramesBuzzJoints = getKeyRotFrames(fileName);
		if (modelSelected == 4)
			keyFramesBuzz = getKeyFrames(fileName);
	}
	if (availableSave && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		saveFrame = true;
		availableSave = false;
	}if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		availableSave = true;

	// Dart Lego model movements
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg -= 0.02;
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrixDart = glm::rotate(modelMatrixDart, 0.02f, glm::vec3(0, 1, 0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrixDart = glm::rotate(modelMatrixDart, -0.02f, glm::vec3(0, 1, 0));
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(-0.02, 0.0, 0.0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(0.02, 0.0, 0.0));

	//Movimientos Buzz
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotBuzzHead += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotBuzzHead -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotBuzzLeftArm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotBuzzLeftArm -= 0.02;
	//Brazo derecho
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotBuzzRightArm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotBuzzRightArm -= 0.02;
	//Mano derecha
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotBuzzRightHand += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotBuzzRightHand -= 0.02;
	//Pierda izquierda
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotBuzzLeftThigh += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotBuzzLeftThigh -= 0.02;
	//Pierna derecha 
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotBuzzRightThigh += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
		glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotBuzzRightThigh -= 0.02;
	//*********Desplazamiento Buzz***********
	if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrixBuzz = glm::rotate(modelMatrixBuzz, 0.02f, glm::vec3(0, 1, 0));
	else if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrixBuzz = glm::rotate(modelMatrixBuzz, -0.02f, glm::vec3(0, 1, 0));
	if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(0.0, 0.0, 0.02));
	else if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(0.0, 0.0, -0.02));


	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(27.5, 0, 30.0));
	modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(180.0f), glm::vec3(0, 1, 0));
	// Variables para la máquina de estados del lambo
	int state = 0;
	float advanceCount = 0.0;
	float rotCount = 0.0;
	float rotWheelsX = 0.0;
	float rotWheelsY = 0.0;
	int numberAdvance = 0;
	int maxAdvance = 0.0;

	int stateLambo = 0;
	float advanceCountLambo = 0.0;
	float rotCountLambo = 0.0;
	float rotWheelsXLambo = 0.0;
	float rotWheelsYLambo = 0.0;
	int numberAdvanceLambo = 0;
	int maxAdvanceLambo = 0.0;

	matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 2.0));

	modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(5.0, 10.0, -5.0));

	modelMatrixAircraft = glm::translate(modelMatrixAircraft, glm::vec3(10.0, 2.0, -17.5));

	modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(23.0, 0.0, 0.0));

	modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(3.0, 0.0, 20.0));

	modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(-1.0, 0.0, -10.0));
	// Hazmat
	modelMatrixHazmat = glm::translate(modelMatrixHazmat, glm::vec3(16.0, 0.0, -10.0));

	//Rose
	modelMatrixRose = glm::translate(modelMatrixRose, glm::vec3(-3.0, 0.0, -5.0));
	modelMatrixRose = glm::scale(modelMatrixRose, glm::vec3(0.05, 0.05, 0.05));


	// Variables to interpolation key frames
	fileName = "../animaciones/animation_dart_joints.txt";
	keyFramesDartJoints = getKeyRotFrames(fileName);
	keyFramesDart = getKeyFrames("../animaciones/animation_dart.txt");

	//Variables para interpolar keyframes buzz
	keyFramesBuzzJoints = getKeyRotFrames("../animaciones/animation_buzz_joints.txt");
	keyFramesBuzz = getKeyFrames("../animaciones/animation_buzz.txt");

	lastTime = TimeManager::Instance().GetTime();

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if (currTime - lastTime < 0.016666667) {//60 frames por segundo
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::vector<float> matrixBuzzJoints;
		std::vector<glm::mat4> matrixBuzz;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / (float)screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
			glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
			glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con multiples texturas
		shaderMultiplesTexturas.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderMultiplesTexturas.setMatrix4("view", 1, false, glm::value_ptr(view));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		shaderMulLighting.setInt("spotLightCount", 0);

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", 0);
		//Ejercicio 
		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMultiplesTexturas.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMultiplesTexturas.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMultiplesTexturas.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMultiplesTexturas.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
		shaderMultiplesTexturas.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		shaderMultiplesTexturas.setInt("spotLightCount", 0);

		shaderMultiplesTexturas.setInt("pointLightCount", 0);


		/*******************************************
		 * Cesped
		 *******************************************/
		glm::mat4 modelCesped = glm::mat4(1.0);
		modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
		modelCesped = glm::scale(modelCesped, glm::vec3(200.0, 0.001, 200.0));
		// Se activa la textura del agua
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureCespedID);
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(200, 200)));
		boxCesped.render(modelCesped);
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		/*******************************************
		 * Casa
		 *******************************************/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureWallID);
		// Frontal wall
		boxWalls.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-10.0, 2.5, 0.0));
		boxWalls.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxWalls.render();
		// Left wall
		boxWalls.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-15.0, 2.5, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Right walls
		// First wall
		boxWalls.setScale(glm::vec3(3.75, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 2.5, -1.875));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Second wall
		boxWalls.setScale(glm::vec3(3.75, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 2.5, -8.125));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Third wall
		boxWalls.setScale(glm::vec3(2.5, 1.25, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 4.375, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Four wall
		boxWalls.setScale(glm::vec3(2.5, 1.25, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 0.625, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Back wall
		boxWalls.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-10.0, 2.5, -10.0));
		boxWalls.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxWalls.render();
		// Ceilling
		boxWalls.setScale(glm::vec3(12.0, 12.0, 0.05));
		boxWalls.setPosition(glm::vec3(-10.0, 5, -5.0));
		boxWalls.setOrientation(glm::vec3(90.0, 0.0, 0.0));
		boxWalls.render();
		// Window
		glBindTexture(GL_TEXTURE_2D, textureWindowID);
		boxWalls.setScale(glm::vec3(2.5, 2.5, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 2.5, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Highway 0
		glBindTexture(GL_TEXTURE_2D, textureHighwayID);
		boxHighway.setScale(glm::vec3(40.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(0.0, 0.05, 10.0));
		boxHighway.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxHighway.render();
		// Highway 1
		boxHighway.setScale(glm::vec3(80.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(25.0, 0.05, 0.0));
		boxHighway.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2, 1)));
		boxHighway.render();
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		// Highway 2
		boxHighway.setScale(glm::vec3(80.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(-25.0, 0.05, 0.0));
		boxHighway.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2, 1)));
		boxHighway.render();
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		// Highway 3
		boxHighway.setScale(glm::vec3(40.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(0.0, 0.05, -35.0));
		boxHighway.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxHighway.render();
		glBindTexture(GL_TEXTURE_2D, 0);
		//Landing pand
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureLandingPadID);
		boxLandingPad.setScale(glm::vec3(10.0f, 0.05, 10.0f));
		boxLandingPad.setPosition(glm::vec3(5.0, 0.05, -5.0));
		boxLandingPad.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(1, 1)));
		boxLandingPad.render();
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		//Render ejercicio multiples texturas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureWaterID);
		shaderMultiplesTexturas.setInt("texture1", 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureSpongeID);
		shaderMultiplesTexturas.setInt("texture2", 1);

		boxMultTexturas.setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
		boxMultTexturas.render();


		///*******************************************
		// * Esfera render
		// *******************************************/
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, textureWindowID);
		//esfera1.setScale(glm::vec3(1.5f, 1.5f, 1.5f));
		//esfera1.setPosition(glm::vec3(0.0f, 2.0f, -5.0f));
		//esfera1.render();

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, textureHighwayID);
		//esfera1.setScale(glm::vec3(3.0f, 3.0f, 3.0f));
		//esfera1.setPosition(glm::vec3(10.0f, 2.0f, -5.0f));
		//esfera1.render();
		//glBindTexture(GL_TEXTURE_2D, 0);

		/*******************************************
		 * Custom objects obj
		 *******************************************/
		 //Rock render
		modelRock.render(matrixModelRock);
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		// Render for the aircraft model
		modelAircraft.render(modelMatrixAircraft);

		// Render for the eclipse car
		glm::mat4 modelMatrixEclipseChasis = glm::mat4(modelMatrixEclipse);
		modelMatrixEclipseChasis = glm::scale(modelMatrixEclipse, glm::vec3(0.5, 0.5, 0.5));
		modelEclipseChasis.render(modelMatrixEclipseChasis);

		glm::mat4 modelMatrixFrontalWheels = glm::mat4(modelMatrixEclipseChasis);
		modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, 1.05813, 4.11483));
		modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsY, glm::vec3(0, 1, 0));
		modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsX, glm::vec3(1, 0, 0));
		modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, -1.05813, -4.11483));
		modelEclipseFrontalWheels.render(modelMatrixFrontalWheels);

		glm::mat4 modelMatrixRearWheels = glm::mat4(modelMatrixEclipseChasis);
		modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, 1.05813, -4.35157));
		modelMatrixRearWheels = glm::rotate(modelMatrixRearWheels, rotWheelsX, glm::vec3(1, 0, 0));
		modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, -1.05813, 4.35157));
		modelEclipseRearWheels.render(modelMatrixRearWheels);

		// Helicopter
		glm::mat4 modelMatrixHelicopChasis = glm::mat4(modelMatrixHeli);
		modelMatrixHelicopChasis = glm::translate(modelMatrixHelicopChasis, glm::vec3(movHeliX, movHeliY, movHeliZ));
		modelHeliChasis.render(modelMatrixHelicopChasis);

		glm::mat4 modelMatrixHelicopHelice1 = glm::mat4(modelMatrixHelicopChasis);
		modelMatrixHelicopHelice1 = glm::translate(modelMatrixHelicopHelice1, glm::vec3(0.0, 0.0, -0.249548));
		modelMatrixHelicopHelice1 = glm::rotate(modelMatrixHelicopHelice1, rotHelHelY, glm::vec3(0, 1, 0));
		modelMatrixHelicopHelice1 = glm::translate(modelMatrixHelicopHelice1, glm::vec3(0.0, 0.0, 0.249548));
		modelHeliHeli.render(modelMatrixHelicopHelice1);

		glm::mat4 modelMatrixHelicopHelice2 = glm::mat4(modelMatrixHelicopChasis);
		modelMatrixHelicopHelice2 = glm::translate(modelMatrixHelicopHelice2, glm::vec3(0.344, 2.09, -5.649));
		modelMatrixHelicopHelice2 = glm::rotate(modelMatrixHelicopHelice2, rotHelHelX, glm::vec3(1, 0, 0));
		modelMatrixHelicopHelice2 = glm::translate(modelMatrixHelicopHelice2, glm::vec3(-0.344, -2.09, 5.649));
		modelMiniHeli.render(modelMatrixHelicopHelice2);

		// Lambo car
		glDisable(GL_CULL_FACE);
		glm::mat4 modelMatrixLamboChasis = glm::mat4(modelMatrixLambo);
		modelMatrixLamboChasis = glm::scale(modelMatrixLamboChasis, glm::vec3(1.3, 1.3, 1.3));
		modelLambo.render(modelMatrixLamboChasis);
		glActiveTexture(GL_TEXTURE0);
		glm::mat4 modelMatrixLamboLeftDor = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(1.0877, 0.70828, 0.979548));
		modelMatrixLamboLeftDor = glm::rotate(modelMatrixLamboLeftDor, glm::radians(dorRotCount), glm::vec3(1.0, 0, 0));
		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(-1.0877, -0.70828, -0.979548));
		//Llantas frontales
		glm::mat4 modelMatrixLamboFrontLeftWheel = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboFrontLeftWheel = glm::translate(modelMatrixLamboFrontLeftWheel, glm::vec3(1.0763, 0.3676, 1.4079));
		modelMatrixLamboFrontLeftWheel = glm::rotate(modelMatrixLamboFrontLeftWheel, rotWheelsYLambo, glm::vec3(0.0, 1.0, 0));
		modelMatrixLamboFrontLeftWheel = glm::rotate(modelMatrixLamboFrontLeftWheel, rotWheelsXLambo, glm::vec3(1.0, 0.0, 0));
		modelMatrixLamboFrontLeftWheel = glm::translate(modelMatrixLamboFrontLeftWheel, glm::vec3(-1.0763, -0.3676, -1.4079));
		modelLamboFrontLeftWheel.render(modelMatrixLamboFrontLeftWheel);

		glm::mat4 modelMatrixLamboFrontRightWheel = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboFrontRightWheel = glm::translate(modelMatrixLamboFrontRightWheel, glm::vec3(-1.0048, 0.3773, 1.39879));
		modelMatrixLamboFrontRightWheel = glm::rotate(modelMatrixLamboFrontRightWheel, rotWheelsYLambo, glm::vec3(0.0, 1.0, 0));
		modelMatrixLamboFrontRightWheel = glm::rotate(modelMatrixLamboFrontRightWheel, rotWheelsXLambo, glm::vec3(1.0, 0.0, 0));
		modelMatrixLamboFrontRightWheel = glm::translate(modelMatrixLamboFrontRightWheel, glm::vec3(1.0048, -0.3773, -1.39879));
		modelLamboFrontRightWheel.render(modelMatrixLamboFrontRightWheel);

		//Llantas Traseras
		glm::mat4 modelMatrixLamboRearLeftWheel = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboRearLeftWheel = glm::translate(modelMatrixLamboRearLeftWheel, glm::vec3(1.0236, 0.39756, -1.6014));
		modelMatrixLamboRearLeftWheel = glm::rotate(modelMatrixLamboRearLeftWheel, rotWheelsXLambo, glm::vec3(1.0, 0.0, 0));
		modelMatrixLamboRearLeftWheel = glm::translate(modelMatrixLamboRearLeftWheel, glm::vec3(-1.0236, -0.39756, 1.6014));
		modelLamboRearLeftWheel.render(modelMatrixLamboRearLeftWheel);

		glm::mat4 modelMatrixLamboRearRightWheel = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboRearRightWheel = glm::translate(modelMatrixLamboRearRightWheel, glm::vec3(-1.009, 0.39756, -1.60014));
		modelMatrixLamboRearRightWheel = glm::rotate(modelMatrixLamboRearRightWheel, rotWheelsXLambo, glm::vec3(1.0, 0.0, 0));
		modelMatrixLamboRearRightWheel = glm::translate(modelMatrixLamboRearRightWheel, glm::vec3(1.009, -0.39756, 1.60014));
		modelLamboRearRightWheel.render(modelMatrixLamboRearRightWheel);

		modelLamboLeftDor.render(modelMatrixLamboLeftDor);
		modelLamboRightDor.render(modelMatrixLamboChasis);

		// Se regresa el cull faces IMPORTANTE para las puertas
		glEnable(GL_CULL_FACE);

		// Dart lego
		// Se deshabilita el cull faces IMPORTANTE para la capa
		glDisable(GL_CULL_FACE);
		glm::mat4 modelMatrixDartBody = glm::mat4(modelMatrixDart);
		modelMatrixDartBody = glm::scale(modelMatrixDartBody, glm::vec3(0.5, 0.5, 0.5));
		modelDartLegoBody.render(modelMatrixDartBody);
		glm::mat4 modelMatrixDartHead = glm::mat4(modelMatrixDartBody);
		modelMatrixDartHead = glm::rotate(modelMatrixDartHead, rotDartHead, glm::vec3(0, 1, 0));
		modelDartLegoHead.render(modelMatrixDartHead);
		modelDartLegoMask.render(modelMatrixDartHead);
		glm::mat4 modelMatrixDartLeftArm = glm::mat4(modelMatrixDartBody);
		modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(-0.023515, 2.43607, 0.446066));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, rotDartLeftArm, glm::vec3(0, 0, 1));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(0.023515, -2.43607, -0.446066));
		modelDartLegoLeftArm.render(modelMatrixDartLeftArm);
		glm::mat4 modelMatrixDartLeftHand = glm::mat4(modelMatrixDartLeftArm);
		modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(0.201343, 1.68317, 0.99774));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, rotDartLeftHand, glm::vec3(0, 1, 0));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(-0.201343, -1.68317, -0.99774));
		modelDartLegoLeftHand.render(modelMatrixDartLeftHand);
		glm::mat4 modelMatrixDartRightArm = glm::mat4(modelMatrixDartBody);
		modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(-0.023515, 2.43607, -0.446066));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, rotDartRightArm, glm::vec3(0, 0, 1));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(0.023515, -2.43607, 0.446066));
		modelDartLegoRightArm.render(modelMatrixDartRightArm);
		glm::mat4 modelMatrixDartRightHand = glm::mat4(modelMatrixDartRightArm);
		modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(0.201343, 1.68317, -0.99774));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, rotDartRightHand, glm::vec3(0, 1, 0));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(-0.201343, -1.68317, 0.99774));
		modelDartLegoRightHand.render(modelMatrixDartRightHand);
		glm::mat4 modelMatrixDartLeftLeg = glm::mat4(modelMatrixDartBody);
		modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, 1.12632, 0.423349));
		modelMatrixDartLeftLeg = glm::rotate(modelMatrixDartLeftLeg, rotDartLeftLeg, glm::vec3(0, 0, 1));
		modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, -1.12632, -0.423349));
		modelDartLegoLeftLeg.render(modelMatrixDartLeftLeg);
		glm::mat4 modelMatrixDartRightLeg = glm::mat4(modelMatrixDartBody);
		modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, 1.12632, -0.423349));
		modelMatrixDartRightLeg = glm::rotate(modelMatrixDartRightLeg, rotDartRightLeg, glm::vec3(0, 0, 1));
		modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, -1.12632, 0.423349));
		modelDartLegoRightLeg.render(modelMatrixDartRightLeg);
		// Se regresa el cull faces IMPORTANTE para la capa
		glEnable(GL_CULL_FACE);

		//************* Buzz *************************************
		glm::mat4 modelMatrixBuzzBody = glm::mat4(modelMatrixBuzz);
		modelMatrixBuzzBody = glm::scale(modelMatrixBuzzBody, glm::vec3(2.5f, 2.5f, 2.5f));
		modelBuzzTorso.render(modelMatrixBuzzBody);
		glm::mat4 modelMatrixBuzzLeftArm = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzLeftArm = glm::translate(modelMatrixBuzzLeftArm, glm::vec3(0.183784f, 0.579864f, -0.027445f));
		modelMatrixBuzzLeftArm = glm::rotate(modelMatrixBuzzLeftArm, rotBuzzLeftArm, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrixBuzzLeftArm = glm::rotate(modelMatrixBuzzLeftArm, glm::radians(-65.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrixBuzzLeftArm = glm::translate(modelMatrixBuzzLeftArm, glm::vec3(-0.183784f, -0.579864f, 0.027445f));
		modelBuzzLeftArm.render(modelMatrixBuzzLeftArm);
		glm::mat4 modelMatrixBuzzLeftForeArm = glm::mat4(modelMatrixBuzzLeftArm);//para que se mueva todo junto
		modelBuzzLeftForeArm.render(modelMatrixBuzzLeftForeArm);//se une el buzz
		glm::mat4 modelMatrixBuzzLeftHand = glm::mat4(modelMatrixBuzzLeftForeArm);
		modelBuzzLeftHand.render(modelMatrixBuzzLeftHand);
		glm::mat4 modelMatrixBuzzHead = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzHead = glm::rotate(modelMatrixBuzzHead, rotBuzzHead, glm::vec3(0, 1.0f, 0.0f));
		modelBuzzHead.render(modelMatrixBuzzHead);
		//**********Brazo derecho completo **********************
		glm::mat4 modelMatrixBuzzRightArm = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzRightArm = glm::translate(modelMatrixBuzzRightArm, glm::vec3(-0.186992f, 0.579864f, -0.027445f));
		modelMatrixBuzzRightArm = glm::rotate(modelMatrixBuzzRightArm, rotBuzzRightArm, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrixBuzzRightArm = glm::rotate(modelMatrixBuzzRightArm, glm::radians(65.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrixBuzzRightArm = glm::translate(modelMatrixBuzzRightArm, glm::vec3(0.186992f, -0.579864f, 0.027445f));
		modelBuzzRightArm.render(modelMatrixBuzzRightArm);
		glm::mat4 modelMatrixBuzzRightForeArm = glm::mat4(modelMatrixBuzzRightArm);//para que se mueva todo junto
		modelBuzzRightForeArm.render(modelMatrixBuzzRightForeArm);//se une el buzz
		glm::mat4 modelMatrixBuzzRightHand = glm::mat4(modelMatrixBuzzRightForeArm);
		modelMatrixBuzzRightHand = glm::translate(modelMatrixBuzzRightHand, glm::vec3(-0.407129, 0.586459, 0.067628));
		modelMatrixBuzzRightHand = glm::rotate(modelMatrixBuzzRightHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixBuzzRightHand = glm::rotate(modelMatrixBuzzRightHand, rotBuzzRightHand, glm::vec3(1, 0, 0));
		modelMatrixBuzzRightHand = glm::rotate(modelMatrixBuzzRightHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixBuzzRightHand = glm::translate(modelMatrixBuzzRightHand, glm::vec3(0.407129, -0.586459, -0.067628));
		modelBuzzRightHand.render(modelMatrixBuzzRightHand);
		// Cadera de Buzz
		glm::mat4 modelMatrixBuzzHip = glm::mat4(modelMatrixBuzzBody);
		modelBuzzHip.render(modelMatrixBuzzHip);
		//Pierna izquierda 
		glm::mat4 modelMatrixBuzzLeftThigh = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzLeftThigh = glm::translate(modelMatrixBuzzLeftThigh, glm::vec3(0.067768, 0.491928, 0.011122));
		modelMatrixBuzzLeftThigh = glm::rotate(modelMatrixBuzzLeftThigh, rotBuzzLeftThigh, glm::vec3(1, 0, 0));
		modelMatrixBuzzLeftThigh = glm::translate(modelMatrixBuzzLeftThigh, glm::vec3(-0.067768, -0.491928, -0.011122));
		modelBuzzLeftThigh.render(modelMatrixBuzzLeftThigh);
		glm::mat4 modelMatrixBuzzLeftCalf = glm::mat4(modelMatrixBuzzLeftThigh);//para que se mueva todo junto
		modelBuzzLeftCalf.render(modelMatrixBuzzLeftCalf);//se une el buzz
		glm::mat4 modelMatrixBuzzLeftFoot = glm::mat4(modelMatrixBuzzLeftCalf);
		modelBuzzLeftFoot.render(modelMatrixBuzzLeftFoot);
		//Pierna derecha
		glm::mat4 modelMatrixBuzzRightThigh = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzRightThigh = glm::translate(modelMatrixBuzzRightThigh, glm::vec3(-0.062676, 0.491928, 0.011122));
		modelMatrixBuzzRightThigh = glm::rotate(modelMatrixBuzzRightThigh, rotBuzzRightThigh, glm::vec3(1, 0, 0));
		modelMatrixBuzzRightThigh = glm::translate(modelMatrixBuzzRightThigh, glm::vec3(0.062676, -0.491928, -0.011122));
		modelBuzzRightThigh.render(modelMatrixBuzzRightThigh);
		glm::mat4 modelMatrixBuzzRightCalf = glm::mat4(modelMatrixBuzzRightThigh);//para que se mueva todo junto
		modelBuzzRightCalf.render(modelMatrixBuzzRightCalf);//se une el buzz
		glm::mat4 modelMatrixBuzzRightFoot = glm::mat4(modelMatrixBuzzRightCalf);
		modelBuzzRightFoot.render(modelMatrixBuzzRightFoot);
		//Ala izquierda
		glm::mat4 modelMatrixBuzzLeftWing1 = glm::mat4(modelMatrixBuzzBody);
		modelBuzzLeftWing1.render(modelMatrixBuzzLeftWing1);
		glm::mat4 modelMatrixBuzzLeftWing2 = glm::mat4(modelMatrixBuzzLeftWing1);
		modelBuzzLeftWing2.render(modelMatrixBuzzLeftWing1);
		//Ala derecha
		glm::mat4 modelMatrixBuzzRightWing1 = glm::mat4(modelMatrixBuzzBody);
		modelBuzzRightWing1.render(modelMatrixBuzzRightWing1);
		glm::mat4 modelMatrixBuzzRightWing2 = glm::mat4(modelMatrixBuzzRightWing1);
		modelBuzzRightWing2.render(modelMatrixBuzzRightWing1);


		//Hazmat
		glm::mat4 modelMatrixHazmatComplete = glm::mat4(modelMatrixHazmat);
		modelHazmat.render(modelMatrixHazmatComplete);

		// Rose
		glm::mat4 modelMatrixRoseComplete = glm::mat4(modelMatrixRose);
		modelRose.render(modelMatrixRoseComplete);

		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

		//Para salvar los frames
		if (record && modelSelected == 1) {
			matrixDartJoints.push_back(rotDartHead);
			matrixDartJoints.push_back(rotDartLeftArm);
			matrixDartJoints.push_back(rotDartLeftHand);
			matrixDartJoints.push_back(rotDartRightArm);
			matrixDartJoints.push_back(rotDartRightHand);
			matrixDartJoints.push_back(rotDartLeftLeg);
			matrixDartJoints.push_back(rotDartRightLeg);
			if (saveFrame) {
				appendFrame(myfile, matrixDartJoints);
				saveFrame = false;
			}
		}
		else if (keyFramesDartJoints.size() > 0) {
			//Para reproducir los frames
			interpolationDartJoints = numPasosDartJoints / (float)maxNumPasosDartJoints;
			numPasosDartJoints++;
			if (interpolationDartJoints > 1.0) {
				numPasosDartJoints = 0;
				interpolationDartJoints = 0;
				indexFrameDartJoints = indexFrameDartJointsNext;
				indexFrameDartJointsNext++;
			}
			if (indexFrameDartJointsNext > keyFramesDartJoints.size() - 1)
				indexFrameDartJointsNext = 0;
			rotDartHead = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 0, interpolationDartJoints);
			rotDartLeftArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 1, interpolationDartJoints);
			rotDartLeftHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 2, interpolationDartJoints);
			rotDartRightArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 3, interpolationDartJoints);
			rotDartRightHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 4, interpolationDartJoints);
			rotDartLeftLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 5, interpolationDartJoints);
			rotDartRightLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 6, interpolationDartJoints);
		}

		if (record && modelSelected == 2) {
			matrixDart.push_back(modelMatrixDart);
			if (saveFrame) {
				appendFrame(myfile, matrixDart);
				saveFrame = false;
			}
		}
		else if (keyFramesDart.size() > 0) {
			//Para reproducir el frame
			interpolationDart = numPasosDart / (float)maxNumPasosDart;
			numPasosDart++;
			if (interpolationDart > 1.0) {
				numPasosDart = 0;
				interpolationDart = 0;
				indexFrameDart = indexFrameDartNext;
				indexFrameDartNext++;
			}
			if (indexFrameDartNext > keyFramesDart.size() - 1)
				indexFrameDartNext = 0;
			modelMatrixDart = interpolate(keyFramesDart, indexFrameDart, indexFrameDartNext, 0, interpolationDart);
		}
		//Para salvar los frames de Buzz
		if (record && modelSelected == 3) {
			matrixBuzzJoints.push_back(rotBuzzHead);
			matrixBuzzJoints.push_back(rotBuzzLeftArm);
			matrixBuzzJoints.push_back(rotBuzzLeftHand);
			matrixBuzzJoints.push_back(rotBuzzRightArm);
			matrixBuzzJoints.push_back(rotBuzzRightHand);
			matrixBuzzJoints.push_back(rotBuzzLeftThigh);
			matrixBuzzJoints.push_back(rotBuzzRightThigh);
			if (saveFrame) {
				appendFrame(myfile, matrixBuzzJoints);
				saveFrame = false;
			}
		}
		else if (keyFramesBuzzJoints.size() > 0) {
			//Para reproducir los frames de buzz
			interpolationBuzzJoints = numPasosBuzzJoints / (float)maxNumPasosBuzzJoints;
			numPasosBuzzJoints++;
			if (interpolationBuzzJoints > 1.0) {
				numPasosBuzzJoints = 0;
				interpolationBuzzJoints = 0;
				indexFrameBuzzJoints = indexFrameBuzzJointsNext;
				indexFrameBuzzJointsNext++;
			}
			if (indexFrameBuzzJointsNext > keyFramesBuzzJoints.size() - 1)
				indexFrameBuzzJointsNext = 0;
			rotBuzzHead = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 0, interpolationBuzzJoints);
			rotBuzzLeftArm = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 1, interpolationBuzzJoints);
			rotBuzzLeftHand = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 2, interpolationBuzzJoints);
			rotBuzzRightArm = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 3, interpolationBuzzJoints);
			rotBuzzRightHand = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 4, interpolationBuzzJoints);
			rotBuzzLeftThigh = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 5, interpolationBuzzJoints);
			rotBuzzRightThigh = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 6, interpolationBuzzJoints);
		}

		if (record && modelSelected == 4) {
			matrixBuzz.push_back(modelMatrixBuzz);
			if (saveFrame) {
				appendFrame(myfile, matrixBuzz);
				saveFrame = false;
			}
		}
		else if (keyFramesBuzz.size() > 0) {
			//Para reproducir el frame
			interpolationBuzz = numPasosBuzz / (float)maxNumPasosBuzz;
			numPasosBuzz++;
			if (interpolationBuzz > 1.0) {
				numPasosBuzz = 0;
				interpolationBuzz = 0;
				indexFrameBuzz = indexFrameBuzzNext;
				indexFrameBuzzNext++;
			}
			if (indexFrameBuzzNext > keyFramesBuzz.size() - 1)
				indexFrameBuzzNext = 0;
			modelMatrixBuzz = interpolate(keyFramesBuzz, indexFrameBuzz, indexFrameBuzzNext, 0, interpolationBuzz);
		}

		// Constantes de animaciones
		//rotHelHelY += 0.5;// ira mas lento si aumentamos ceros despues del 5
		//rotHelHelX += 0.5;

		/*******************************************
		 * Maquinas de estado
		 *******************************************/
		 //**************************************************************************************************
		 //Maquina de estados del helicóptero

		 //Velocidad de hélices de helicoptero
		if (rotHelix == 0) {
			rotHelHelY += 0.4;
			rotHelHelX += 0.4;
		}
		else if (rotHelix == 1) { // ...Descendiendo
			rotHelHelY += 0.05;
			rotHelHelX += 0.05;
		}
		else { // Helices detenidas
			rotHelHelY = 0.0;
			rotHelHelX = 0.0;
		}

		//Movimiento del helicoptero completo
		switch (heliState) {

			//Aterrizando helicoptero (movimiento descendente en eje Y)
		case 0:
			movHeliY -= 0.02;
			if (movHeliY <= -10.0) {
				heliState = 1;
				rotHelix = 1; // Se detiene 
			}
			break;

		case 1: // Completamente aterrizado
			stopHeli += 0.05;
			if (stopHeli >= 10.0) {
				rotHelix = 2;
			}
			else if (stopHeli >= 20.0) {
				stopHeli = 0.0;
				heliState = 2;//Se detienen las helices
				rotHelix = 0;
			}

			break;
		}
		//**************************************************************************************************

		// Maquina de estado del eclipse
		switch (state) {
		case 0:
			if (numberAdvance == 0)
				maxAdvance = 65.0f;
			else if (numberAdvance == 1)
				maxAdvance = 49.0;
			else if (numberAdvance == 2)
				maxAdvance = 44.5;
			else if (numberAdvance == 3)
				maxAdvance = 49.0;
			else if (numberAdvance == 4)
				maxAdvance = 44.5;
			state = 1;
			break;
		case 1:
			modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0f, 0.0f, 0.1));
			advanceCount += 0.1;
			rotWheelsX += 0.05;
			rotWheelsY -= 0.02;
			if (rotWheelsY < 0)
				rotWheelsY = 0;
			if (advanceCount > maxAdvance) {
				advanceCount = 0;
				numberAdvance++;
				state = 2;
			}
			break;
		case 2:
			modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0f, 0.0f, 0.025f));
			modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
			rotCount += 0.5f; //
			rotWheelsX += 0.05;
			rotWheelsY += 0.02;
			if (rotWheelsY > 0.24)
				rotWheelsY = 0.24;
			if (rotCount >= 90.0f) {
				rotCount = 0;
				state = 0;
				if (numberAdvance > 4)
					numberAdvance = 1;
			}
			break;
		default:
			break;
		}

		//**************************************************************************************************

		// Maquina de estado del Lambo
		switch (stateLambo) {
		case 0:
			if (numberAdvanceLambo == 0)
				maxAdvanceLambo = 5.0f;
			else if (numberAdvanceLambo == 1)
				maxAdvanceLambo = 40.0;
			else if (numberAdvanceLambo == 2)
				maxAdvanceLambo = 35.0;
			else if (numberAdvanceLambo == 3)
				maxAdvanceLambo = 40.0;
			else if (numberAdvanceLambo == 4)
				maxAdvanceLambo = 30.0;
			stateLambo = 1;
			break;
		case 1:
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.0f, 0.0f, 0.1));
			printf("avance lambo:%f max:%d\n", advanceCountLambo, maxAdvanceLambo);
			advanceCountLambo += 0.1;
			rotWheelsXLambo += 1.0;
			rotWheelsYLambo -= 0.02;
			if (rotWheelsYLambo < 0)
				rotWheelsYLambo = 0;
			if (advanceCountLambo > maxAdvanceLambo) {
				advanceCountLambo = 0;
				//Revisar si es el ultimo punto del circuito
				if (numberAdvanceLambo >= 4)
					stateLambo = 3;
				else
					stateLambo = 2;
				numberAdvanceLambo++;
			}
			break;
		case 2:
			modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.0f, 0.0f, 0.025f));
			modelMatrixLambo = glm::rotate(modelMatrixLambo, glm::radians(-0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
			rotCountLambo -= 0.5f; //
			rotWheelsXLambo += 2.0;
			rotWheelsYLambo -= 0.05;
			if (rotWheelsYLambo < -0.5f)
				rotWheelsYLambo = -0.5f;
			if (rotCountLambo <= -90.0f) {
				rotCountLambo = 0;
				stateLambo = 0;
				if (numberAdvanceLambo > 4)
					numberAdvanceLambo = 1;
			}
			break;
		case 3:
			// Maquina de estados de la puerta del lambo
			switch (stateDoor) {
			case 0:
				dorRotCount += 0.5;
				if (dorRotCount > 75.0f)
					dorRotCount = 75.0f;
				break;
				//case 1:
				//	dorRotCount -= 0.5;
				//	if (dorRotCount < 0) {
				//		dorRotCount = 0.0f;
				//		//stateDoor = 0;
				//	}
			default:
				break;
			}
		default:
			break;
		}
		//std::cout << "Maximo avance:=>" << advanceCount << std::endl; //para visualizar avance del carro 

		glfwSwapBuffers(window);
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	//Tamaño ventana, titulo de la pantalla, y modo completo
	applicationLoop();
	destroy();
	return 1;
}