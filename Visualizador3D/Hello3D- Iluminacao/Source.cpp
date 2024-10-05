// Trabalho GrauA CG2024-2
// Nome: Rodrigo Fuelber Franke

/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para as disciplinas de Processamento Gráfico/Computação Gráfica - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 12/08/2024
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Classe gerenciadora de shaders
#include "Shader.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int loadSimpleOBJ(string filePATH, int &nVertices, glm::vec3 color);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 1920, HEIGHT = 1080;

int modelo = 1;

//Variáveis globais da câmera
glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f,0.0,-1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f,1.0f,0.0f);

struct Object
{
	GLuint VAO; //Índice do buffer de geometria
	int nVertices; //nro de vértices
	glm::mat4 model; //matriz de transformações do objeto
    bool rotateX = false;
    bool rotateY = false;
    bool rotateZ = false;
    float angle = 0.0f;
    float escala = 1.0f;
	glm::vec3 movimento;
};

std::vector<Object> objects;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();


	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Visualizador 3D ----- GrauA", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	Shader shader("phong.vs","phong.fs");


    glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);

	Object obj1;
	obj1.VAO = loadSimpleOBJ("objects/Suzanne.obj",obj1.nVertices, red);
	
	objects.push_back(obj1);

	Object obj2;
	obj2.VAO = loadSimpleOBJ("objects/cube.obj", obj2.nVertices, green);
	
	objects.push_back(obj2);

	Object obj3;
	obj3.VAO = loadSimpleOBJ("objects/Nave.obj", obj3.nVertices, blue);
	
	objects.push_back(obj3);


	glUseProgram(shader.ID);

	//Matriz de modelo
	glm::mat4 model = glm::mat4(1); //matriz identidade;
	GLint modelLoc = glGetUniformLocation(shader.ID, "model");
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//Matriz de view
	glm::mat4 view = glm::lookAt(cameraPos,glm::vec3(0.0f,0.0f,0.0f),cameraUp);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	//Matriz de projeção
	glm::mat4 projection = glm::perspective(glm::radians(39.6f),(float)WIDTH/HEIGHT,0.1f,100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glEnable(GL_DEPTH_TEST);



	//Propriedades da superfície

	// coeficiente de reflexao ambiente (quanto da luz ambiente é refletida pelo objeto)
	shader.setFloat("ka",0.15);

	// coeficiente de reflexao especular (circulos brilhantes que aparecem em superficies)
	shader.setFloat("ks", 0.9);

	// coeficiente de reflexao difusa (fração da luz incidente a ser refletida/espalhada na superfície)
	shader.setFloat("kd", 0.35);

	// exponente de especularidade (controla a nitidez dos reflexos especulares)
	shader.setFloat("q", 100.0);

	//Propriedades da fonte de luz
	shader.setVec3("lightPos",-2.0, 10.0, 3.0);
	shader.setVec3("lightColor",1.0, 1.0, 1.0);




	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{


		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);


        for (int i = 0; i < objects.size(); ++i)
        {
            Object& objeto = objects[i];
        
            objeto.model = glm::mat4(1); //matriz identidade 

			objeto.model = glm::translate(objeto.model, objeto.movimento); // Aplica a movimentacao dos objetos

            objeto.angle = (GLfloat)glfwGetTime();

            // Aplica a rotacao do objeto
            if (objeto.rotateX)
            {
                objeto.model = glm::rotate(objeto.model, objeto.angle, glm::vec3(1.0f, 0.0f, 0.0f));
            }
            else if (objeto.rotateY)
            {
                objeto.model = glm::rotate(objeto.model, objeto.angle, glm::vec3(0.0f, 1.0f, 0.0f));
            }
            else if (objeto.rotateZ)
            {
                objeto.model = glm::rotate(objeto.model, objeto.angle, glm::vec3(0.0f, 0.0f, 1.0f));
            }
			
			// Aplica a escala aos objetos
            objeto.model = glm::scale(objeto.model, glm::vec3(objeto.escala, objeto.escala, objeto.escala));


            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(objeto.model));


			//Atualizar a matriz de view
			//Matriz de view
			glm::mat4 view = glm::lookAt(cameraPos,cameraPos + cameraFront,cameraUp);
			glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));


			//Propriedades da câmera
			shader.setVec3("cameraPos",cameraPos.x, cameraPos.y, cameraPos.z);


			// Chamada de desenho - drawcall
			// Poligono Preenchido - GL_TRIANGLES
            glBindVertexArray(objeto.VAO);
            glDrawArrays(GL_TRIANGLES, 0, objeto.nVertices);
        }

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}


	// Pede pra OpenGL desalocar os buffers
	for(Object objeto : objects){
		glDeleteVertexArrays(1, &objeto.VAO);
	}
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);



	// Pega o numero selecionado pelo usuario
	if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9 && action == GLFW_PRESS)
	{
		modelo = key - GLFW_KEY_0;
	}



	// Aplica a rotacao no eixo
	if (key == GLFW_KEY_X && action == GLFW_PRESS) // Eixo X
    {
        objects[modelo - 1].rotateX = true;
        objects[modelo - 1].rotateY = false;
        objects[modelo - 1].rotateZ = false;
    }

    if (key == GLFW_KEY_Y && action == GLFW_PRESS) // Eixo Y
    {
        objects[modelo - 1].rotateX = false;
        objects[modelo - 1].rotateY = true;
        objects[modelo - 1].rotateZ = false;
    }

    if (key == GLFW_KEY_Z && action == GLFW_PRESS) // Eixo Z
    {
        objects[modelo - 1].rotateX = false;
        objects[modelo - 1].rotateY = false;
        objects[modelo - 1].rotateZ = true;
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) // Para a rotacao
    {
        objects[modelo - 1].rotateX = false;
        objects[modelo - 1].rotateY = false;
        objects[modelo - 1].rotateZ = false;
    }





	// Movimenta a camera
	float cameraSpeed = 0.05f;

	if ((key == GLFW_KEY_W) && action == GLFW_PRESS) // Frente
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	if ((key == GLFW_KEY_S) && action == GLFW_PRESS) // Tras
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	if ((key == GLFW_KEY_A) && action == GLFW_PRESS) // Esquerda
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if ((key == GLFW_KEY_D) && action == GLFW_PRESS)  // Direita
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

    if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS) // Sobe
    {
        cameraPos += cameraSpeed * cameraUp;
    }
    if ((key == GLFW_KEY_LEFT_CONTROL) && action == GLFW_PRESS) // Desce
    {
        cameraPos -= cameraSpeed * cameraUp;
    }

	


	// Altera a escala do Objeto
	float escalaSize = 0.1f;

    if (key == GLFW_KEY_P && action == GLFW_PRESS) // Aumenta
    {
        objects[modelo - 1].escala += escalaSize;
    }

    if (key == GLFW_KEY_M && action == GLFW_PRESS) // Diminui
    {
        objects[modelo - 1].escala = glm::max(objects[modelo - 1].escala - escalaSize, 0.1f);
    }




	// Movimenta o Objeto
	float objetoSpeed =  0.1f;

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) // Esquerda
    {
        objects[modelo - 1].movimento.x -= objetoSpeed;
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) // Direita
    {
        objects[modelo - 1].movimento.x += objetoSpeed;
    }

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) // Sobe
    {
        objects[modelo - 1].movimento.y += objetoSpeed;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) // Desce
    {
        objects[modelo - 1].movimento.y -= objetoSpeed;
    }

    if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS) // Frente
    {
        objects[modelo - 1].movimento.z += objetoSpeed;
    }
    if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS) // Tras
    {
        objects[modelo - 1].movimento.z -= objetoSpeed;
    }


}


int loadSimpleOBJ(string filePath, int &nVertices, glm::vec3 color)
{
	vector <glm::vec3> vertices;
	vector <glm::vec2> texCoords;
	vector <glm::vec3> normals;
	vector <GLfloat> vBuffer;

	ifstream arqEntrada;

	arqEntrada.open(filePath.c_str());
	if (arqEntrada.is_open())
	{
		//Fazer o parsing
		string line;
		while (!arqEntrada.eof())
		{
			getline(arqEntrada,line);
			istringstream ssline(line);
			string word;
			ssline >> word;
			if (word == "v")
			{
				glm::vec3 vertice;
				ssline >> vertice.x >> vertice.y >> vertice.z;
				vertices.push_back(vertice);

			}
			if (word == "vt")
			{
				glm::vec2 vt;
				ssline >> vt.s >> vt.t;
				texCoords.push_back(vt);

			}
			if (word == "vn")
			{
				glm::vec3 normal;
				ssline >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal);

			}
			else if (word == "f")
			{
				while (ssline >> word) 
				{
					int vi, ti, ni;
					istringstream ss(word);
    				std::string index;

    				// Pega o índice do vértice
    				std::getline(ss, index, '/');
    				vi = std::stoi(index) - 1;  // Ajusta para índice 0

    				// Pega o índice da coordenada de textura
    				std::getline(ss, index, '/');
    				ti = std::stoi(index) - 1;

    				// Pega o índice da normal
    				std::getline(ss, index);
    				ni = std::stoi(index) - 1;

					//Recuperando os vértices do indice lido
					vBuffer.push_back(vertices[vi].x);
					vBuffer.push_back(vertices[vi].y);
					vBuffer.push_back(vertices[vi].z);
					
					//Atributo cor
					vBuffer.push_back(color.r);
					vBuffer.push_back(color.g);
					vBuffer.push_back(color.b);

					//Atributo coordenada de textura
					vBuffer.push_back(texCoords[ti].s);
					vBuffer.push_back(texCoords[ti].t);

					//Atributo vetor normal
					vBuffer.push_back(normals[ni].x);
					vBuffer.push_back(normals[ni].y);
					vBuffer.push_back(normals[ni].z);
    			}
				
			}
		}

		arqEntrada.close();

		cout << "Gerando o buffer de geometria..." << endl;
		GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, vBuffer.size() * sizeof(GLfloat), vBuffer.data(), GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	
	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo coordenada de textura - s, t
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Atributo vetor normal - x, y, z
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8*sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	nVertices = vBuffer.size() / 2;
	return VAO;

	}
	else
	{
		cout << "Erro ao tentar ler o arquivo " << filePath << endl;
		return -1;
	}
}
