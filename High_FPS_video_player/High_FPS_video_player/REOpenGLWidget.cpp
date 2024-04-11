#include "REOpenGLWidget.h"
#include "shader_arr.h"

REOpenGLWidget::REOpenGLWidget(QWidget* parent)
	: QOpenGLWidget( parent ), dataSingleton(DataSingleton::getInstance())
{
	frameQueue = dataSingleton.getVideoFrameQueue();

	// ��ʱ������ָ��  (Ҫ�����Ӳۺ���֮ǰ������ʱ������)
	frameTimer = new QTimer(this);
	countTimer = new QTimer(this);
	// ��ʱ����
	frameTimer->setTimerType(Qt::PreciseTimer);
	countTimer->setTimerType(Qt::PreciseTimer);
	// ��ʱ���
	frameTimer->setInterval(8);
	countTimer->setInterval(1000);
	// ����ѭ����ʱ
	frameTimer->setSingleShot(false);
	countTimer->setSingleShot(false);

	frameTimer->start();
	countTimer->start();
	// �����źźͲ�
	connectSignalSlots();

	// ��ɫ�����������
	shaderArr = new ShaderArr();

	frameN = av_frame_alloc();
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// ����VAO����ĵ�һ������
	glEnableVertexAttribArray(0);
	// �������ݽ����ʽ
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	// ����VAO����ĵ�һ������
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenTextures(1, &texture0);



	videoFrameTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
	videoFrameTexture->create();
	videoFrameTexture->bind();
	// �����������
	videoFrameTexture->setMinificationFilter(QOpenGLTexture::Linear);
	videoFrameTexture->setMagnificationFilter(QOpenGLTexture::Linear);
	videoFrameTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
	videoFrameTexture->release();
}

void REOpenGLWidget::resizeGL(int w, int h)
{

}

// �����źźͲ�
void REOpenGLWidget::connectSignalSlots()
{
	connect(this->frameTimer, &QTimer::timeout, this, &REOpenGLWidget::flush);
	connect(this->countTimer, &QTimer::timeout, [this] { qDebug() << this->count;
	this->count = 0; });
}

void REOpenGLWidget::paintGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// ��VAO
	glBindVertexArray(VAO);

	videoFrameTexture->bind();

	shaderArr->useShaderId(videoShaderKey);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	videoFrameTexture->release();
	// ���VAO
	glBindVertexArray(0);
}

void REOpenGLWidget::flush()
{
	frameQueue->waitAndPop(frame);
	//frameQueue->tryPop(frame);

	if (!codecCtx)
	{
		codecCtx = dataSingleton.getVCodecCtx();
		swsContext = sws_getCachedContext(swsContext, codecCtx->width, codecCtx->height, codecCtx->pix_fmt, codecCtx->width, codecCtx->height, AV_PIX_FMT_RGB24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
	}
	if (!outBuffer)
		outBuffer = (uint8_t*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB24, codecCtx->width, codecCtx->height, 1));
	av_image_fill_arrays(frameN->data, frameN->linesize, outBuffer, AV_PIX_FMT_RGB24, codecCtx->width, codecCtx->height, 1);

	sws_scale(swsContext, frame->data, frame->linesize, 0, frame->height, frameN->data, frameN->linesize);


	videoFrameTexture->bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, codecCtx->width, codecCtx->height, 0, GL_RGB, GL_UNSIGNED_BYTE, frameN->data[0]);
	videoFrameTexture->release();


	++count;
	update();

	
	av_frame_free(&frame);
}