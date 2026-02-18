#include "filterruletest.h"
#include <QtWidgets/QApplication>

class FilterRuleTest;

bool enableGLFunction(void* arg)
{
    if(arg)
    {
        ((FilterRuleTest*)arg)->getCanvas()->makeCurrent();
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FilterRuleTest w;
	w.setMinimumSize(640, 480);
	w.show();
	w.setWindowTitle(QString::fromLocal8Bit("Filter Rule Validation Tool V2.0.0"));

	CGE_LOG_INFO("After completing an operation, please click Save. A series of files will be generated. Open the _cge.txt file, copy the text, paste it into the text box above, and click the center button to verify the rule is correct.\n\n"
		"The text box below will display the final effect result under this rule.\n"
		"Click the Compare Original button in the upper right to see the difference from the original image.\n\n"
		"In additional options, set the resource image to use. By default it uses the same image as FilterGenerator.\n"
		"If the effect uses resources, set the resource directory in additional options.\n"
		"After clicking Save Result Image, a result image will be output in the same directory as the input image.\n");
    CGE::cgeSetGLContextEnableFunction(enableGLFunction, &w);
	return a.exec();
}
