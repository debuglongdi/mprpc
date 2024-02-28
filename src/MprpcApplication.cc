#include "MprpcApplication.h"


void MprpcApplication::init(int argc, char **argv)
{

}

MprpcApplication& MprpcApplication::getInstance()
{
    static MprpcApplication mp;
    return mp;
}

MprpcApplication::MprpcApplication()
{

}