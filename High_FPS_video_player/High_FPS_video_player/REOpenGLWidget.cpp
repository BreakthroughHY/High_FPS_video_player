#include "REOpenGLWidget.h"
#include "shader_arr.h"

REOpenGLWidget::REOpenGLWidget(QWidget* parent)
	: QOpenGLWidget( parent ), dataSingleton(DataSingleton::getInstance())
{
	frameQueue = dataSingleton.getVideoFrameQueue();

	codecCtx = dataSingleton.getACodecCtx();

	// 定时器对象指针  (要在连接槽函数之前创建计时器对象)
	frameTimer = new QTimer(this);
	// 定时精度
	frameTimer->setTimerType(Qt::PreciseTimer);
	// 定时间隔
	frameTimer->setInterval(1);
	// 设置循环计时
	frameTimer->setSingleShot(false);

	frameTimer->start();
	// 连接信号和槽
	connectSignalSlots();

	// 着色程序数组对象
	shaderArr = new ShaderArr();
}

REOpenGLWidget::~REOpenGLWidget() 
{
	delete shaderArr;
}

// 虚函数需要重写
void REOpenGLWidget::initializeGL()
{
	// 找到显卡函数
	initializeOpenGLFunctions();

	// 加载视频纹理着色器
	videoShaderKey = shaderArr->loadShader(".\ShaderFile\shader.vs", ".\ShaderFile\shader.fs");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// 绑定VBO和VAO对象
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 设置数据解读方式
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// 开启VAO管理的第一个属性
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//// 创建顶点着色器
	//unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//glCompileShader(vertexShader);

	//// 创建片段着色器对象
	//unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//glCompileShader(fragmentShader);

	//// 链接着色器对象
	//shaderProgram = glCreateProgram();
	//glAttachShader(shaderProgram, vertexShader);
	//glAttachShader(shaderProgram, fragmentShader);
	//glLinkProgram(shaderProgram);

	//glDeleteShader(vertexShader);
	//glDeleteShader(fragmentShader);

	glGenTextures(1, &texture0);
}

void REOpenGLWidget::resizeGL(int w, int h)
{

}

// 连接信号和槽
void REOpenGLWidget::connectSignalSlots()
{
	connect(this->frameTimer, &QTimer::timeout, this, &REOpenGLWidget::flush);
}

void REOpenGLWidget::paintGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	// 设置纹理对应片段着色器中的变量
	int vertexMultiplierLocation = glGetUniformLocation(shaderArr->getShaderId(videoShaderKey), "videoFrame");
	glUniform1i(vertexMultiplierLocation, 0);


	shaderArr->useShaderId(videoShaderKey);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void REOpenGLWidget::flush()
{
	frameQueue->waitAndPop(frame);
	swsContext = sws_getCachedContext(swsContext, codecCtx->width, codecCtx->height, codecCtx->pix_fmt, codecCtx->width, codecCtx->height, AV_PIX_FMT_RGB24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
	if (!outBuffer)
		outBuffer = (uint8_t*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB24, codecCtx->width, codecCtx->height, 64));
	av_image_fill_arrays(dst_data, dst_linesize, outBuffer, AV_PIX_FMT_RGB24, codecCtx->width, codecCtx->height, 1);

	sws_scale(swsContext, frame->data, frame->linesize, 0, frame->height, dst_data, dst_linesize);

	glBindTexture(GL_TEXTURE_2D, texture0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, codecCtx->width, codecCtx->height, 0, GL_RGB, GL_UNSIGNED_BYTE, dst_data);

	//frameQueue->tryPop(frame);
	av_frame_free(&frame);
}