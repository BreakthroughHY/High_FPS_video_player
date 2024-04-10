#include "REOpenGLWidget.h"
#include "shader_arr.h"

REOpenGLWidget::REOpenGLWidget(QWidget* parent)
	: QOpenGLWidget( parent ), dataSingleton(DataSingleton::getInstance())
{
	frameQueue = dataSingleton.getVideoFrameQueue();

	codecCtx = dataSingleton.getACodecCtx();

	// ��ʱ������ָ��  (Ҫ�����Ӳۺ���֮ǰ������ʱ������)
	frameTimer = new QTimer(this);
	// ��ʱ����
	frameTimer->setTimerType(Qt::PreciseTimer);
	// ��ʱ���
	frameTimer->setInterval(1);
	// ����ѭ����ʱ
	frameTimer->setSingleShot(false);

	frameTimer->start();
	// �����źźͲ�
	connectSignalSlots();

	// ��ɫ�����������
	shaderArr = new ShaderArr();
}

REOpenGLWidget::~REOpenGLWidget() 
{
	delete shaderArr;
}

// �麯����Ҫ��д
void REOpenGLWidget::initializeGL()
{
	// �ҵ��Կ�����
	initializeOpenGLFunctions();

	// ������Ƶ������ɫ��
	videoShaderKey = shaderArr->loadShader(".\ShaderFile\shader.vs", ".\ShaderFile\shader.fs");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// ��VBO��VAO����
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// �������ݽ����ʽ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// ����VAO����ĵ�һ������
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//// ����������ɫ��
	//unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//glCompileShader(vertexShader);

	//// ����Ƭ����ɫ������
	//unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//glCompileShader(fragmentShader);

	//// ������ɫ������
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

// �����źźͲ�
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
	// ���������ӦƬ����ɫ���еı���
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