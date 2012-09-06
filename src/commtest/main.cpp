
// libusb
#include <usb.h>
// std lib
#include <iostream>



usb_dev_handle* getUsbDeviceHandle(size_t vendor, size_t product);
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
int main(int argc, char* argv[])
{
	const static size_t ID_CTECHNOLOGY = 0x0a93;
	const static size_t ID_CPEN10      = 0x0002;

	usb_dev_handle* const pDevHnd = getUsbDeviceHandle(ID_CTECHNOLOGY, ID_CPEN10);
	if(NULL == pDevHnd)
	{
		std::cout << "device not found!" << std::endl;
		return 1;
	}

	const size_t GET_VERSION = 0xb0; // 12 bytes string
	uint8_t buf[256];
	const int timeout = 1000;
	const int index = 0x00;
//	usb_control_msg(pDevHnd, 0xC0, GET_VERSION, 0x00, index, (char*)buf, 12, timeout);
//	std::cout << "version string : " << buf << std::endl;




	return 0;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
usb_dev_handle* getUsbDeviceHandle(const size_t vendor, const size_t product)
{
	usb_init();
	usb_find_busses();
	usb_find_devices();

	usb_bus* busses = usb_get_busses();

	for(struct usb_bus* bus = busses; bus; bus = bus->next)
	{
		for(struct usb_device* devi = bus->devices; devi; devi = devi->next)
		{
			if(vendor == devi->descriptor.idVendor && product == devi->descriptor.idProduct)
			{
				usb_dev_handle* handle = usb_open(devi);
				if(!handle)
					return 0;
//				if(usb_claim_interface(handle, 0) < 0)
//					return 0;

				return handle;
			}
		}
	}
	return 0;
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A

