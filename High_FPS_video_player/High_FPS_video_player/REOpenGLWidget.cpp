#include "REOpenGLWidget.h"
#include "shader_arr.h"

REOpenGLWidget::REOpenGLWidget(QWidget* parent)
	: QOpenGLWidget( parent ), dataSingleton(DataSingleton::getInstance())
{
	frameQueue = dataSingleton.getVideoFrameQueue();

	// 定时器对象指针  (要在连接槽函数之前创建计时器对象)
	frameTimer = new QTimer(this);
	countTimer = new QTimer(this);
	// 定时精度
	frameTimer->setTimerType(Qt::PreciseTimer);
	countTimer->setTimerType(Qt::PreciseTimer);
	// 定时间隔
	frameTimer->setInterval(4);
	countTimer->setInterval(1000);
	// 设置循环计时
	frameTimer->setSingleShot(false);
	countTimer->setSingleShot(false);

	// 连接信号和槽
	connectSignalSlots();

	// 着色程序数组对象
	shaderArr = new ShaderArr();

	frameN = av_frame_alloc();
}

REOpenGLWidget::~REOpenGLWidget() 
{
	delete shaderArr;
}

// 设置属性
void REOpenGLWidget::setParameters()
{
	codecCtx = dataSingleton.getVCodecCtx();
	// 设置视频时间基
	vTimeBase = dataSingleton.getvTimeBase();
	// 设置pts
	beforePTS = currPTS = 0;
	// 定时间隔
	frameTimer->setInterval((int)(1000 / dataSingleton.getFPSV()));
	frameTimer->start();
	countTimer->start();
}

// 开始渲染
void REOpenGLWidget::start()
{
	frameTimer->start();
	countTimer->start();
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// 开启VAO管理的第一个属性
	glEnableVertexAttribArray(0);
	// 设置数据解读方式
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	// 开启VAO管理的第一个属性
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenTextures(1, &texture0);



	videoFrameTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
	videoFrameTexture->create();
	videoFrameTexture->bind();
	// 设置纹理参数
	videoFrameTexture->setMinificationFilter(QOpenGLTexture::Linear);
	videoFrameTexture->setMagnificationFilter(QOpenGLTexture::Linear);
	videoFrameTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
	videoFrameTexture->release();
}

void REOpenGLWidget::resizeGL(int w, int h)
{

}

// 连接信号和槽
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

	// 绑定VAO
	glBindVertexArray(VAO);

	videoFrameTexture->bind();

	shaderArr->useShaderId(videoShaderKey);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	videoFrameTexture->release();
	// 解绑VAO
	glBindVertexArray(0);
}

void REOpenGLWidget::flush()
{
	if (!myFrame)
	{
		//frameQueue->waitAndPop(myFrame); // 一定要取出一个帧
		if (!frameQueue->tryPop(myFrame)) // 尝试取出一个帧
		{
			return;
		}
	}


	while (myFrame->pts * av_q2d(vTimeBase) < beforePTS)
	{
		qDebug() << myFrame->pts * av_q2d(vTimeBase) << "dq" << beforePTS;
		av_freep(&myFrame->outBuffer);
		delete myFrame;
		frameQueue->waitAndPop(myFrame);
	}

	if (myFrame->pts * av_q2d(vTimeBase) > currPTS)
	{
		dataSingleton.getPTS(beforePTS, currPTS);
		if (myFrame->pts * av_q2d(vTimeBase) > currPTS)
			return;
	}

	videoFrameTexture->bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, codecCtx->width, codecCtx->height, 0, GL_RGB, GL_UNSIGNED_BYTE, myFrame->data[0]);
	videoFrameTexture->release();

	++count;
	this->update();

	av_freep(&myFrame->outBuffer);
	delete myFrame;
	myFrame = nullptr;
}

