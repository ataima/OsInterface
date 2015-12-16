////////////////////////////////////////////////////////////////////////////////
//    Copyright (C) 2011  Angelo Coppi (coppi dot angelo at virgilio dot it )
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////
// Author : Angelo Coppi (coppi dot angelo at virgilio dot it )
// History:        
// Created on 13 Nov 2011
////////////////////////////////////////////////////////////////////////////////

#include "../inc/device_configure.h"
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "../inc/IFile.h"
#include "../inc/CDefaultFile.h"
#include "../inc/CGenericDevice.h"

// test of device class

bool test1(IDevice *device)
{
    bool res = false;
    std::cout << "CDefaultFileTest test 1" << std::endl;
    if (device == NULL)
    {
        std::cout << "%TEST_FAILED% CDefaultFileTest test 1 message=device=NULL !!" << std::endl;
    }
    else
    {
        if (device != device::CGenericDevice::getDevice())
        {
            std::cout << "%TEST_FAILED% CDefaultFileTest test 1 message=device!=instance !!" << std::endl;
        }
        else
        {
            ILogger *log = device->GetAvaiableLogger();
            if (log == NULL)
            {
                std::cout << "%TEST_FAILED% CDefaultFileTest test 1 message=LOG is NULL" << std::endl;
            }
            else
            {
                IHardware *hw = device->GetAvaiableHardware();
                if (hw == NULL)
                {
                    std::cout << "%TEST_FAILED% CDefaultFileTest test 1 message=hw is NULL" << std::endl;
                }
                else
                {
                    ISoftware *sw = device->GetAvaiableSoftware();
                    if (sw == NULL)
                    {
                        std::cout << "%TEST_FAILED% CDefaultFileTest test 1 message=sw is NULL" << std::endl;
                    }
                    else
                    {
                        res = true;
                    }
                }
            }
        }
    }
    return res;
}

bool test2(IDevice *device)
{
    bool res = false;
    std::cout << "CDefaultFileTest test 2" << std::endl;
    if (device == NULL)
    {
        std::cout << "%TEST_FAILED% CDefaultFileTest test 2 message=device=NULL !!" << std::endl;
    }
    else
    {
        int i;
#if     _HAVE_COM_
        IHardware *hw = device->GetAvaiableHardware();
        if (hw != NULL)
        {
            for (i = 0; i < TOTAL_COM_PORT; i++)
            {
                ICommPort *com = hw->GetAvaiableSerialPort(i);
                if (com == NULL)
                {
                    std::cout << "%TEST_FAILED% CDefaultFileTest test 2 message=COM(" << i << ")=NULL !!" << std::endl;
                    break;
                }
            }
        }
        else
        {
            std::cout << "%TEST_FAILED% CDefaultFileTest test 2 message=hardware=NULL !!" << std::endl;
        }
#endif        
    }
}

bool test3(IDevice *device)
{
    bool res = false;
    std::cout << "CDefaultFileTest test 3" << std::endl;
    if (device == NULL)
    {
        std::cout << "%TEST_FAILED% CDefaultFileTest test 3 message=device=NULL !!" << std::endl;
    }
    else
    {
        int i;
#if     _HAVE_SPI_
        IHardware *hw = device->GetAvaiableHardware();
        if (hw != NULL)
        {
            for (i = 0; i < TOTAL_SPI_PORT; i++)
            {
                ISpiPort *spi = hw->GetAvaiableSpiPort(i);
                if (spi == NULL)
                {
                    std::cout << "%TEST_FAILED% CDefaultFileTest test 3 message=SPI(" << i << ")=NULL !!" << std::endl;
                    break;
                }
            }
        }
        else
        {
            std::cout << "%TEST_FAILED% CDefaultFileTest test 3 message=hardware=NULL !!" << std::endl;
        }
#endif        
    }
}



void test10()
{
    int res = 0;
    std::cout << "CDefaultFileTest test 10" << std::endl;
    IFile *file = new CDefaultFile();
    res = file->Open("./CDefaultFileTest_10.txt", write_on);
    if (res != 0)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 10 Open FAIL" << std::endl;
    if (file->IsOpen() == false)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 10 IsOpen FAIL" << std::endl;
    res = file->Close();
    if (res != 0)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 10 Close FAIL" << std::endl;
    if (file->GetFilename() == NULL)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 10 GetFileName FAIL" << std::endl;
    else
        remove(file->GetFilename());
    delete file;
}

void test11()
{
    int res = 0;
    std::cout << "CDefaultFileTest test 11" << std::endl;
    IFile *file = new CDefaultFile();
    res = file->Open("./CDefaultFileTest_11.txt", write_on);
    if (res != 0)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 11 Open FAIL" << std::endl;
    if (file->IsOpen() == false)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 11 IsOpen FAIL" << std::endl;
    res = file->Close();
    if (res != 0)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 11 Close FAIL" << std::endl;
    if (file->GetFilename() == NULL)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 11 GetFileName FAIL" << std::endl;
    else
        remove(file->GetFilename());
    delete file;
}

void test12()
{
    int res = 0;
    std::cout << "CDefaultFileTest test 12" << std::endl;
    IFile *file = new CDefaultFile();
    res = file->Open("./CDefaultFileTest_12.txt", write_on);
    if (res != 0)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 12 Open FAIL" << std::endl;
    if (file->IsOpen() == false)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 12 IsOpen FAIL" << std::endl;
    unsigned char out[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    unsigned int writed = -1;
    if (file->Write(out, sizeof (out), writed) != 0)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 12 Write FAIL" << std::endl;
    if (writed != sizeof (out))
        std::cout << "%TEST_FAILED% CDefaultFileTest test 12 Write size FAIL" << std::endl;
    res = file->Close();
    if (res != 0)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 12 Close FAIL" << std::endl;
    if (file->GetFilename() == NULL)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 12 GetFileName FAIL" << std::endl;
    else
        remove(file->GetFilename());
    delete file;
}

void test13()
{
    int res = 0;
    std::cout << "CDefaultFileTest test 13" << std::endl;
    IFile *file = new CDefaultFile();
    res = file->Open("./CDefaultFileTest_13.txt", write_on);
    if (res != 0)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 13 Open FAIL" << std::endl;
    if (file->IsOpen() == false)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 13 IsOpen FAIL" << std::endl;
    unsigned char out[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    unsigned int writed = -1;
    if (file->Write(out, sizeof (out), writed) != 0)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 13 Write FAIL" << std::endl;
    if (writed != sizeof (out))
        std::cout << "%TEST_FAILED% CDefaultFileTest test 13 Write size FAIL" << std::endl;
    res = file->Flush();
    if (res != 0)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 13 flush FAIL" << std::endl;
    res = file->Close();
    if (res != 0)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 13 Close FAIL" << std::endl;
    res = file->Open("./CDefaultFileTest_13.txt", read_on);
    unsigned char in[16];
    memset(in, 0, sizeof (in));
    if (file->Read(in, 6, writed, 10000) != 0)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 13 Read  FAIL" << std::endl;
    if (writed != 6)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 13 Read size FAIL" << std::endl;
    if (memcmp(out, in, 6) != 0)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 13 Read cmp FAIL" << std::endl;
    res = file->Close();
    if (res != 0)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 13 Close FAIL" << std::endl;
    if (file->GetFilename() == NULL)
        std::cout << "%TEST_FAILED% CDefaultFileTest test 13 GetFileName FAIL" << std::endl;
    else
        remove(file->GetFilename());
    delete file;
}

int main(int argc, char** argv)
{
    IDevice *device = device::CGenericDevice::Create("stdout");

    std::cout << "%SUITE_STARTING% CDefaultFileTest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (CDefaultFileTest)" << std::endl;
    test1(device);
    std::cout << "%TEST_FINISHED% time=0 test1 (CDefaultFileTest)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (CDefaultFileTest)" << std::endl;
    test2(device);
    std::cout << "%TEST_FINISHED% time=0 test2 (CDefaultFileTest)" << std::endl;

    std::cout << "%TEST_STARTED% test3 (CDefaultFileTest)" << std::endl;
    test3(device);
    std::cout << "%TEST_FINISHED% time=0 test3 (CDefaultFileTest)" << std::endl;

    std::cout << "%TEST_STARTED% test10 (CDefaultFileTest)" << std::endl;
    test10();
    std::cout << "%TEST_FINISHED% time=0 test10 (CDefaultFileTest)" << std::endl;

    std::cout << "%TEST_STARTED% test11 (CDefaultFileTest)" << std::endl;
    test11();
    std::cout << "%TEST_FINISHED% time=0 test11 (CDefaultFileTest)" << std::endl;

    std::cout << "%TEST_STARTED% test12 (CDefaultFileTest)" << std::endl;
    test12();
    std::cout << "%TEST_FINISHED% time=0 test12 (CDefaultFileTest)" << std::endl;

    std::cout << "%TEST_STARTED% test13 (CDefaultFileTest)" << std::endl;
    test13();
    std::cout << "%TEST_FINISHED% time=0 test13 (CDefaultFileTest)" << std::endl;


    std::cout << "%SUITE_FINISHED% time=0" << std::endl;
    device = device::CGenericDevice::Destroy();
    return (EXIT_SUCCESS);
}

