#ifndef INP_DEVICES_H
#define INP_DEVICES_H

#include <windows.h>
#include <iostream>
#include <QString>

QString get_inp_dev()
{
    QString list = "";

    // Get Number Of Devices
    UINT nDevices = 0;
    GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST));

    // Got Any?
    if (nDevices < 1)
    {
        // Exit
        std::cout << "ERR: 0 Devices?";
        return "";
    }

    // Allocate Memory For Device List
    PRAWINPUTDEVICELIST pRawInputDeviceList;
    pRawInputDeviceList = new RAWINPUTDEVICELIST[sizeof(RAWINPUTDEVICELIST) * nDevices];

    // Got Memory?
    if (pRawInputDeviceList == NULL)
    {
        // Error
        std::cout << "ERR: Could not allocate memory for Device List.";
        return "";
    }

    // Fill Device List Buffer
    int nResult;
    nResult = GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST));

    // Got Device List?
    if (nResult < 0)
    {
        // Clean Up
        delete[] pRawInputDeviceList;

        // Error
        std::cout << "ERR: Could not get device list.";
        return "";
    }

    bool mAdded = false;
    bool kAdded = false;

    // Loop Through Device List
    for (UINT i = 0; i < nDevices; i++)
    {
        // Get Character Count For Device Name
        UINT nBufferSize = 0;
        nResult = GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice, // Device
            RIDI_DEVICENAME,                // Get Device Name
            NULL,                           // NO Buff, Want Count!
            &nBufferSize);                 // Char Count Here!

                                           // Got Device Name?
        if (nResult < 0)
        {
            // Error
            std::cout << "ERR: Unable to get Device Name character count.. Moving to next device.\n\n";

            // Next
            continue;
        }

        // Allocate Memory For Device Name
        WCHAR* wcDeviceName = new WCHAR[nBufferSize + 1];

        // Got Memory
        if (wcDeviceName == NULL)
        {
            // Error
            std::cout << "ERR: Unable to allocate memory for Device Name.. Moving to next device.\n\n";

            // Next
            continue;
        }

        // Get Name
        nResult = GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice, // Device
            RIDI_DEVICENAME,                // Get Device Name
            wcDeviceName,                   // Get Name!
            &nBufferSize);                 // Char Count

                                           // Got Device Name?
        if (nResult < 0)
        {
            // Error
            std::cout << "ERR: Unable to get Device Name.. Moving to next device.\n\n";

            // Clean Up
            delete[] wcDeviceName;

            // Next
            continue;
        }

        // Set Device Info & Buffer Size
        RID_DEVICE_INFO rdiDeviceInfo;
        rdiDeviceInfo.cbSize = sizeof(RID_DEVICE_INFO);
        nBufferSize = rdiDeviceInfo.cbSize;

        // Get Device Info
        nResult = GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice,
            RIDI_DEVICEINFO,
            &rdiDeviceInfo,
            &nBufferSize);

        // Got All Buffer?
        if (nResult < 0)
        {
            // Error
            std::cout << "ERR: Unable to read Device Info.. Moving to next device.\n\n";

            // Next
            continue;
        }

        // Mouse
        if (rdiDeviceInfo.dwType == RIM_TYPEMOUSE && rdiDeviceInfo.mouse.dwNumberOfButtons == 2 && mAdded == false)
        {
            mAdded = true;
            // Current Device
            list.append("\nDisplaying (MOUSE) information.\n");
            //list.append("Device Name: " + QString::fromWCharArray(wcDeviceName) + "\n");
            list.append("\tMouse buttons: " + QString::number(rdiDeviceInfo.mouse.dwNumberOfButtons) + "\n");
            list.append("\tMouse sample rate (Data Points): 1000 Hz");
            //list.append("Mouse sample rate (Data Points): " + QString::number(rdiDeviceInfo.mouse.dwSampleRate));

//            cout << endl << "Displaying device " << i + 1 << " information. (MOUSE)" << endl;
//            wcout << L"Device Name: " << wcDeviceName << endl;
//            cout << "Mouse ID: " << rdiDeviceInfo.mouse.dwId << endl;
//            cout << "Mouse buttons: " << rdiDeviceInfo.mouse.dwNumberOfButtons << endl;
//            cout << "Mouse sample rate (Data Points): " << rdiDeviceInfo.mouse.dwSampleRate << endl;

//            if (rdiDeviceInfo.mouse.fHasHorizontalWheel)
//            {
//                cout << "Mouse has horizontal wheel" << endl;
//            }
//            else
//            {
//                cout << "Mouse does not have horizontal wheel" << endl;
//            }
        }

        // Keyboard
        else if (rdiDeviceInfo.dwType == RIM_TYPEKEYBOARD && rdiDeviceInfo.keyboard.dwNumberOfKeysTotal == 101 && kAdded == false)
        {
            kAdded = true;
            // Current Device
            list.append("\nDisplaying (KEYBOARD) information.\n");
            //list.append("Device Name: " + QString::fromWCharArray(wcDeviceName) + "\n");
            list.append("\tNumber of function keys: " + QString::number(rdiDeviceInfo.keyboard.dwNumberOfFunctionKeys) + "\n");
            list.append("\tNumber of keys total: " + QString::number(rdiDeviceInfo.keyboard.dwNumberOfKeysTotal) + "\n");

//            cout << endl << "Displaying device " << i + 1 << " information. (KEYBOARD)" << endl;
//            wcout << L"Device Name: " << wcDeviceName << endl;
//            cout << "Keyboard mode: " << rdiDeviceInfo.keyboard.dwKeyboardMode << endl;
//            cout << "Number of function keys: " << rdiDeviceInfo.keyboard.dwNumberOfFunctionKeys << endl;
//            cout << "Number of indicators: " << rdiDeviceInfo.keyboard.dwNumberOfIndicators << endl;
//            cout << "Number of keys total: " << rdiDeviceInfo.keyboard.dwNumberOfKeysTotal << endl;
//            cout << "Type of the keyboard: " << rdiDeviceInfo.keyboard.dwType << endl;
//            cout << "Subtype of the keyboard: " << rdiDeviceInfo.keyboard.dwSubType << endl;
        }
        // Delete Name Memory!
        delete[] wcDeviceName;
    }

    // Clean Up - Free Memory
    delete[] pRawInputDeviceList;

    return list;
}

#endif // INP_DEVICES_H
