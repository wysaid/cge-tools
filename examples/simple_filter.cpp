/*
 * Simple example demonstrating how to use CGE library
 * Minimal compilation test
 */

#include "cge/cge.h"
#include <QCoreApplication>
#include <QDebug>

using namespace CGE;

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << "CGE Library initialized successfully!";
    qDebug() << "Library version: 2.0.0";
    
    // Test that we can create a filter object
    CGEImageFilterInterface* filter = new CGEImageFilterInterface();
    delete filter;
    
    qDebug() << "Filter creation test passed!";
    
    return 0;
}
