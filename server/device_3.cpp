static const char *RcsId = "$Id$\n$Name$";

//+============================================================================
//
// file :               Device_3.cpp
//
// description :        C++ source code for the DeviceImpl and DeviceClass
//			classes. These classes
//			are the root class for all derived Device classes.
//			They are abstract classes. The DeviceImpl class is the
//			CORBA servant which is "exported" onto the network and
//			accessed by the client.
//
// project :            TANGO
//
// author(s) :          E.Taurel
//
// $Revision$
//
// $Log$
// Revision 3.24  2007/04/20 14:40:28  taurel
// - Ported to Windows 64 bits x64 architecture
//
// Revision 3.23  2007/04/16 14:56:36  taurel
// - Added 3 new attributes data types (DevULong, DevULong64 and DevState)
// - Ported to omniORB4.1
// - Increased the MAX_TRANSFER_SIZE to 256 MBytes
// - Added a new filterable field in the archive event
//
// Revision 3.22  2007/03/06 08:18:03  taurel
// - Added 64 bits data types for 64 bits computer...
//
// Revision 3.21  2007/02/16 10:40:09  taurel
// - Implementing a new types of event on the Attribute configuration
//
// Revision 3.20  2007/02/08 16:32:56  taurel
// - Remove an old already commented line
//
// Revision 3.19  2007/01/19 10:25:00  taurel
// - Change a logging message to be more precise
//
// Revision 3.18  2006/06/20 08:00:04  jensmeyer
// After changing the the init value of the attribute polling period to -1 insted of 0 I detected
// a campatibility problem with the old versions. I changed back the init value to 0 and
// solved the problem with new methods on the Attribute object.
//
// Revision 3.17  2006/06/14 11:21:35  jensmeyer
// Modifications to make the set_polling_period() for attributes work
// with a polling period set to 0.
//
// Revision 3.16  2006/05/30 11:17:56  jensmeyer
// Modifications on push_event functionality for MS Windows.
// Mainly modifications for timing and time stamps.
//
// Revision 3.15  2006/05/18 08:52:37  taurel
// - Miscellaneous changes due to Python device server ported to Windows
// - Fix some bugs discovered by Windows VC8 using the test suite
// - Update Windows resource file include path
// - Fix some Windows VC8 warnings
//
// Revision 3.14  2006/04/28 13:17:20  taurel
// - Attribute class: Add state and status attribute special case to the method Attribute_2_AttributeValue
// - Device_3Impl: The monitor taken by the read_attribute_history_3 call was the Device Monitor. Replaced it by the polling list monitor
//
// Revision 3.13  2006/01/27 14:27:09  taurel
// - Fix a severe incompatibility problem introduced by all the modifs done for
// PY DS
// - Duplicate some EventSupplier class methods (instead of using template) in order to be able to generate Tango shared library on Suse 9.3
//
// Revision 3.12  2006/01/20 08:25:54  taurel
// - Added necessary changes to support Device server written in Python
//
// Revision 3.11  2005/11/02 12:54:13  jensmeyer
// Avoid calling status2attr() during attribute reading when an error was detected.
// Avoids segmentation fault in status2attr().
//
// Revision 3.10  2005/05/09 15:34:10  taurel
// - Fix deadlock when read_attribute_no_cache try to start attribute polling
// - Signal management for Sun
//
// Revision 3.9  2005/03/14 09:49:29  taurel
// - Fix some bugs in filedatabase (Change in the data transferred between client and
//   server).
// - Fix bug in event re-connection
// - Add event support even for device server started with the -file command line option
//
// Revision 3.8  2005/03/03 15:36:17  taurel
// - Change in attribute quality factor change event. It is now fired by the Attribute
// set_quailty() and set_value_date_quality() methods. For scannable device.
//
// Revision 3.7  2005/01/13 09:27:53  taurel
// Fix some bugs :
// - R/W attribute : W value not returned when read if set by set_write_value
// - Core dumped when retrieving attribute polling history for Device_2Impl device which
//   has stored an exception
// - Remove device_name in lib default attribute label property
// - Lib default value for label not store in db any more
// - Size of the DaData used by the Database::get_device_attribute_property() and
//   Database::get_class_attribute_property()
// - R/W attribute: W part not returned when read for Device_2Impl device
// Some changes :
// - Improvement of the -file option error management (now throw exception in case of
//   error)
// - Reset "string" attribute property to the default value (lib or user) when new
//   value is an empty string
//
// Revision 3.5.2.8  2004/11/26 13:53:20  taurel
// - Fix bug if exception thrown during Util class construction
// - Change attribute label and format default values
//
// Revision 3.5.2.7  2004/11/04 09:46:38  taurel
// - Add a tuning method in the polling thread
// - Some minor fixes to pass test suite
//
// Revision 3.5.2.6  2004/10/27 05:59:46  taurel
// - Some minor changes to compile on all our supported platforms
//
// Revision 3.5.2.5  2004/10/22 11:25:50  taurel
// Added warning alarm
// Change attribute config. It now includes alarm and event parameters
// Array attribute property now supported
// subscribe_event throws exception for change event if they are not correctly configured
// Change in the polling thread: The event heartbeat has its own work in the work list
// Also add some event_unregister
// Fix order in which classes are destructed
// Fix bug in asynchronous mode (PUSH_CALLBACK). The callback thread ate all the CPU
// Change in the CORBA info call for the device type
//
// Revision 3.5.2.4  2004/09/27 09:09:07  taurel
// - Changes to allow reading state and/or status as attributes
//
// Revision 3.5.2.3  2004/09/15 06:45:45  taurel
// - Added four new types for attributes (boolean, float, unsigned short and unsigned char)
// - It is also possible to read state and status as attributes
// - Fix bug in Database::get_class_property() method (missing ends insertion)
// - Fix bug in admin device DevRestart command (device name case problem)
//
// Revision 3.5.2.2  2004/08/19 07:44:04  taurel
// - Replace server low level database access call by Database class method call
// - Split device monitor in 3 : 1 to protect harware access, 1 to protect cache access and one mutex for device black box
//
// Revision 3.5.2.1  2004/07/15 15:04:06  taurel
// - Added the way to externally filled the polling buffer for attribute
//   (Command will come soon)
//
// Revision 3.5  2004/07/07 08:40:11  taurel
//
// - Fisrt commit after merge between Trunk and release 4 branch
// - Add EventData copy ctor, asiignement operator and dtor
// - Add Database and DeviceProxy::get_alias() method
// - Add AttributeProxy ctor from "device_alias/attribute_name"
// - Exception thrown when subscribing two times for exactly yhe same event
//
// Revision 3.4  2004/06/15 12:38:22  taurel
// - Last commit before merge with release 4 branch
//
// Revision 3.3  2003/09/02 13:08:14  taurel
// Add memorized attribute feature (only for SCALAR and WRITE/READ_WRITE attribute)
//
// Revision 3.2  2003/08/22 12:52:54  taurel
// - For device implementing release 3 of IDL (derivating from device_3impl), change
//   the way how attributes are read or written
// - Fix small bug in vector iterator usage in case of "erase()" method called in
//   a for loop
//
// Revision 3.1  2003/08/21 07:37:04  taurel
// - End of the implementation of the new way to transfer data for read and
//   write attributes (better use of exception)
// - Added Attribute::set_date() and Attribute::set_value_date_quality() methods
// - Added DeviceAttribute ctors from "const char *"
// - Enable writing of spectrum and image attributes
// - Many new DeviceAttribute ctors/inserters to enable easy image and spectrums
//   attribute writing
// - Attribute date automatically set in case of attribute quality factor set to INVALID
// - Change in the polling thread discarding element algo. to support case of polling
//   several cmd/atts at the same polling period with cmd/attr having a long response time
// - Take cmd/attr execution time into account in the "Data not updated since" polling
//   status string
// - Split "str().c_str()" code in two lines of code. It was the reason of some problem
//   on Windows device server
// - Add the possibility to set a cmd/attr polling as "externally triggered". Add method
//   to send trigger to the polling thread
//
// Revision 3.0  2003/07/03 07:42:38  taurel
// - New IDL interface (Device3)
//
//
//
// copyleft :           European Synchrotron Radiation Facility
//                      BP 220, Grenoble 38043
//                      FRANCE
//
//-============================================================================

#if HAVE_CONFIG_H
#include <ac_config.h>
#endif

#include <tango.h>
#include <device_3.h>
#include <new>

#include <eventsupplier.h>

#ifdef _TG_WINDOWS_
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif /* _TG_WINDOWS_ */


namespace Tango
{

//+-------------------------------------------------------------------------
//
// method : 		Device_3Impl::Device_3Impl 
// 
// description : 	constructors for the device_impl class from the 
//			class object pointer, the device name, 
//			the description field, the state and the status.
//			Device_3Impl inherits from DeviceImpl. These constructors
//			simply call the correct DeviceImpl class
//			constructor
//
//--------------------------------------------------------------------------

Device_3Impl::Device_3Impl(DeviceClass *device_class,string &dev_name):
Device_2Impl(device_class,dev_name)
{
	ext->idl_version = 3;
	add_state_status_attrs();
	
	ext_3 = new Device_3ImplExt();
}
		
Device_3Impl::Device_3Impl(DeviceClass *device_class,
			   string &dev_name,
			   string &desc):
Device_2Impl(device_class,dev_name,desc)
{
	ext->idl_version = 3;
	add_state_status_attrs();

	ext_3 = new Device_3ImplExt();
}
		
Device_3Impl::Device_3Impl(DeviceClass *device_class,
	           	   string &dev_name,string &desc,
	           	   Tango::DevState dev_state,string &dev_status):
Device_2Impl(device_class,dev_name,desc,dev_state,dev_status),ext_3(NULL)
{
	ext->idl_version = 3;
	add_state_status_attrs();

	ext_3 = new Device_3ImplExt();
} 
		   
Device_3Impl::Device_3Impl(DeviceClass *device_class,
	           	   const char *dev_name,
			   const char *desc,
	           	   Tango::DevState dev_state,
	           	   const char *dev_status):
Device_2Impl(device_class,dev_name,desc,dev_state,dev_status)
{
	ext->idl_version = 3;
	add_state_status_attrs();

	ext_3 = new Device_3ImplExt();
}



//+-------------------------------------------------------------------------
//
// method : 		Device_3Impl::read_attributes_3 
// 
// description : 	Method called for each read_attributes operation executed
//			from any client on a Tango device version 3.
//
//--------------------------------------------------------------------------

Tango::AttributeValueList_3* Device_3Impl::read_attributes_3(const Tango::DevVarStringArray& names,
					     Tango::DevSource source)
throw (Tango::DevFailed, CORBA::SystemException)
{
//	cout4 << "Device_3Impl::read_attributes_3 arrived" << endl;
	cout4 << "Device_3Impl::read_attributes_3 arrived for dev " << get_name() << ", att[0] = " << names[0] << endl;

//
// Record operation request in black box
//

	blackbox_ptr->insert_attr(names,3,source);
			
//
// Build a sequence with the names of the attribute to be read.
// This is necessary in case of the "AllAttr" shortcut is used
// If all attributes are wanted, build this list
//

	unsigned long nb_names = names.length();
	unsigned long nb_dev_attr = dev_attr->get_attr_nb();
	Tango::DevVarStringArray real_names(nb_names);
	unsigned long i;
	
	if (nb_names == 1)
	{
		string att_name(names[0]);				
		if (att_name == AllAttr)
		{
			real_names.length(nb_dev_attr);
			for (i = 0;i < nb_dev_attr;i++)
			{
				real_names[i] = dev_attr->get_attr_by_ind(i).get_name().c_str();
			}
		}
		else
		{
			real_names = names;
		}
	}
	else
	{
		real_names = names;
	}
	nb_names = real_names.length();

//
// Allocate memory for the AttributeValue structures
//

	Tango::AttributeValueList_3 *back;
	try
	{	
		back = new Tango::AttributeValueList_3(nb_names);
		back->length(nb_names);
	}
	catch (bad_alloc)
	{
		Except::throw_exception((const char *)"API_MemoryAllocation",
				        (const char *)"Can't allocate memory in server",
				        (const char *)"Device_3Impl::read_attributes_3");
	}
				
//
// If the source parameter specifies device, call the read_attributes method
// which does not throw exception except for major fault (cant allocate
// memory,....)
//
	vector <long> idx_in_back;
	
	if (source == Tango::DEV)
	{

		try
		{	
			AutoTangoMonitor sync(this);
			read_attributes_no_except(real_names,back,false,idx_in_back);
		}
		catch (...)
		{
			delete back;
			throw;
		}
	}
	else if (source == Tango::CACHE)
	{
		try
		{
			TangoMonitor &mon = get_poll_monitor();
			AutoTangoMonitor sync(&mon);
			read_attributes_from_cache(real_names,back);
		}
		catch (...)
		{
			delete back;
			throw;
		}
	}
	else
	{
	
//
// It must be now CACHE_DEVICE (no other choice), first try to get
// values from cache
//

		try
		{
			TangoMonitor &mon = get_poll_monitor();
			AutoTangoMonitor sync(&mon);
			read_attributes_from_cache(real_names,back);
		}
		catch (...)
		{
			delete back;
			throw;
		}

//
// Now, build the list of attributes which it was not possible
// to get their value from cache
//

		Tango::DevVarStringArray names_from_device(nb_names);
		long nb_attr = 0;

	       	for (i = 0;i < nb_names;i++)
	       	{
			long nb_err = (*back)[i].err_list.length();
	       		if (nb_err != 0)
			{
				nb_err--;
				if ((strcmp((*back)[i].err_list[nb_err].reason,"API_AttrNotPolled") == 0) ||
				    (strcmp((*back)[i].err_list[nb_err].reason,"API_NoDataYet") == 0) ||
				    (strcmp((*back)[i].err_list[nb_err].reason,"API_NotUpdatedAnyMore") == 0) ||
				    (strcmp((*back)[i].err_list[nb_err].origin,"DServer::add_obj_polling") == 0))
				{
					nb_attr++;
					names_from_device.length(nb_attr);
					names_from_device[nb_attr - 1] = real_names[i];
					idx_in_back.push_back(i);
					
					(*back)[i].err_list.length(0);
				}
			}	
	       	}

		if (nb_attr != 0)
		{
				
//
// Try to get their values from device
//

			try
			{
				AutoTangoMonitor sync(this);
				read_attributes_no_except(names_from_device,back,true,idx_in_back);
			}
			catch (...)
			{
				delete back;
				throw;
			}

		}

	}	

	return back;

}


//+-------------------------------------------------------------------------
//
// method : 		Device_3Impl::read_attributes_no_except 
// 
// description : 	Read attributes from device but do not throw exception
//			if it fails. This method is mainly a copy of the original
//			DeviceImpl::read_attributes method.
//
// argument: in :	- names: The names of the attribute to read
//			- back : Pointer to the sequence returned to the client.
//				 Memory has already been allocated for
//				 this pointer
//
//--------------------------------------------------------------------------


void Device_3Impl::read_attributes_no_except(const Tango::DevVarStringArray& names,
					     Tango::AttributeValueList_3 *&back,
					     bool second_try,
					     vector<long> &idx)
{

//
// Retrieve index of wanted attributes in the device attribute list and clear 
// their value set flag
//

	long nb_names = names.length();
	vector<AttIdx> wanted_attr;
	vector<AttIdx> wanted_w_attr;
	bool state_wanted = false;
	bool status_wanted = false;
	long state_idx,status_idx;
	long i;

	for (i = 0;i < nb_names;i++)
	{
		long j;
		AttIdx x;
		x.idx_in_names = i;
		string att_name(names[i]);
		transform(att_name.begin(),att_name.end(),att_name.begin(),::tolower);
		
		if (att_name == "state")
		{
			x.idx_in_multi_attr = -1;
			x.failed = false;
			wanted_attr.push_back(x);
			state_wanted = true;
			state_idx = i;
		}
		else if (att_name == "status")
		{
			x.idx_in_multi_attr = -1;
			x.failed = false;
			wanted_attr.push_back(x);
			status_wanted = true;
			status_idx = i;
		}
		else
		{
			try
			{
				j = dev_attr->get_attr_ind_by_name(names[i]);
				if ((dev_attr->get_attr_by_ind(j).get_writable() == Tango::READ_WRITE) ||
				    (dev_attr->get_attr_by_ind(j).get_writable() == Tango::READ_WITH_WRITE))
				{
					x.idx_in_multi_attr = j;
					x.failed = false;
					wanted_w_attr.push_back(x);
					wanted_attr.push_back(x);
					Attribute &att = dev_attr->get_attr_by_ind(wanted_attr.back().idx_in_multi_attr);
					att.set_value_flag(false);
					att.get_when().tv_sec = 0;
				}
				else
				{
					if (dev_attr->get_attr_by_ind(j).get_writable() == Tango::WRITE)
					{
						x.idx_in_multi_attr = j	;
						x.failed = false;
						wanted_w_attr.push_back(x);
					}
					else
					{
						x.idx_in_multi_attr = j;
						x.failed = false;
						wanted_attr.push_back(x);
						Attribute &att = dev_attr->get_attr_by_ind(wanted_attr.back().idx_in_multi_attr);
						att.set_value_flag(false);
						att.get_when().tv_sec = 0;
					}
				}
			}
			catch (Tango::DevFailed &e)
			{
				long index;
				if (second_try == false)
					index = i;
				else
					index = idx[i];

				(*back)[index].err_list = e.errors;
				(*back)[index].quality = Tango::ATTR_INVALID;
				(*back)[index].name = CORBA::string_dup(names[i]);
				clear_att_dim((*back)[index]);
			}
		}		
	}
	long nb_wanted_attr = wanted_attr.size();
	long nb_wanted_w_attr = wanted_w_attr.size();

//
// Call the always_executed_hook
//

	always_executed_hook();
		
//
// Read the hardware for readable attribute but not for state/status
// Warning:  If the state is one of the wanted attribute, check and
// eventually add all the alarmed attributes index
//

	if (nb_wanted_attr != 0)
	{
		vector<long> tmp_idx;
		for (i = 0;i < nb_wanted_attr;i++)
		{
			long ii = wanted_attr[i].idx_in_multi_attr;
			if (ii != -1)
				tmp_idx.push_back(ii);
		}
		if (state_wanted == true)
		{
			if ((device_state == Tango::ON) || (device_state == Tango::ALARM))
				add_alarmed(tmp_idx);
		}
		
		if (tmp_idx.size() != 0)
			read_attr_hardware(tmp_idx);
	}
	
//
// Set attr value (for readable attribute) but not for state/status
//

	for (i = 0;i < nb_wanted_attr;i++)
	{
		if (wanted_attr[i].idx_in_multi_attr != -1)
		{
			try
			{
				Attribute &att = dev_attr->get_attr_by_ind(wanted_attr[i].idx_in_multi_attr);
				vector<Tango::Attr *> &attr_vect = device_class->get_class_attr()->get_attr_list();
				if (attr_vect[att.get_attr_idx()]->is_allowed(this,Tango::READ_REQ) == false)
				{
					TangoSys_OMemStream o;

					o << "It is actually not allowed to read attribute ";
					o << att.get_name() << ends;

					Except::throw_exception((const char *)"API_AttrNotAllowed",
					        		o.str(),
					        		(const char *)"Device_3Impl::read_attributes_no_except");
				}
				attr_vect[att.get_attr_idx()]->read(this,att);

			}
			catch (Tango::DevFailed &e)
			{
				long index;
				if (second_try == false)
					index = wanted_attr[i].idx_in_names;
				else
					index = idx[wanted_attr[i].idx_in_names];

				wanted_attr[i].failed = true;
				(*back)[index].err_list = e.errors;
				(*back)[index].quality = Tango::ATTR_INVALID;
				(*back)[index].name = CORBA::string_dup(names[wanted_attr[i].idx_in_names]);
				clear_att_dim((*back)[index]);
			}
		}
	}
	
//
// Set attr value for writable attribute
//

	for (i = 0;i < nb_wanted_w_attr;i++)
	{
		try
		{
			Tango::AttrWriteType w_type = dev_attr->get_attr_by_ind(wanted_w_attr[i].idx_in_multi_attr).get_writable();
			if ((w_type == Tango::READ_WITH_WRITE) ||
		    	    (w_type == Tango::WRITE)) 
				dev_attr->get_attr_by_ind(wanted_w_attr[i].idx_in_multi_attr).set_rvalue();
		}
		catch (Tango::DevFailed &e)
		{
			long index;
			if (second_try == false)
				index = wanted_w_attr[i].idx_in_names;
			else
				index = idx[wanted_w_attr[i].idx_in_names];
				
			wanted_w_attr[i].failed = true;
			(*back)[index].err_list = e.errors;		
			(*back)[index].quality = Tango::ATTR_INVALID;
			(*back)[index].name = CORBA::string_dup(names[wanted_w_attr[i].idx_in_names]);
			clear_att_dim((*back)[index]);
		}
	}

//
// If necessary, read state and/or status
// If the device has some alarmed attributes and some of them have already been
// read and failed, it is not necessary to read state, simply copy faulty
// alarmed attribute error message to the state attribute error messages
//

	Tango::DevState d_state;
	Tango::ConstDevString d_status;
	
	if (state_wanted == true)
	{
		if ((device_state == Tango::ON) || (device_state == Tango::ALARM))
		{
			long id = reading_state_necessary(wanted_attr);
			if (id == -1)
			{
				try
				{
					alarmed_not_read(wanted_attr);
					ext->state_from_read = true;
					d_state = dev_state();
					ext->state_from_read = false;
				}
				catch (Tango::DevFailed &e)
				{
					ext->state_from_read = false;
					(*back)[state_idx].err_list = e.errors;		
					(*back)[state_idx].quality = Tango::ATTR_INVALID;
					(*back)[state_idx].name = CORBA::string_dup(names[state_idx]);
					clear_att_dim((*back)[state_idx]);
				}
			}
			else
			{	
				(*back)[state_idx].err_list = (*back)[wanted_attr[id].idx_in_names].err_list;		
				(*back)[state_idx].quality = Tango::ATTR_INVALID;
				(*back)[state_idx].name = CORBA::string_dup(names[state_idx]);
				clear_att_dim((*back)[state_idx]);
			}
		}
		else
			d_state = dev_state();
	}
	
	if (status_wanted == true)
	{
		try
		{
			d_status = dev_status();
		}
		catch (Tango::DevFailed &e)
		{
			(*back)[status_idx].err_list = e.errors;		
			(*back)[status_idx].quality = Tango::ATTR_INVALID;
			(*back)[status_idx].name = CORBA::string_dup(names[status_idx]);
			clear_att_dim((*back)[status_idx]);
		}
	}
			
//
// Build the sequence returned to caller for readable attributes and check 
// that all the wanted attributes set value have been updated
//

	for (i = 0;i < nb_names;i++)
	{
		long index;
		if (second_try == false)
			index = i;
		else
			index = idx[i];

		if ((state_wanted == true) && (state_idx == i))
		{
			if ((*back)[index].err_list.length() == 0)
				state2attr(d_state,(*back)[index]);
			continue;
		}
		
		if ((status_wanted == true) && (status_idx == i))
		{
			if ((*back)[index].err_list.length() == 0)
				status2attr(d_status,(*back)[index]);
			continue;
		}
		
		if ((*back)[index].err_list.length() == 0)
		{
			Attribute &att = dev_attr->get_attr_by_name(names[i]);
			Tango::AttrQuality qual = att.get_quality();
			if (qual != Tango::ATTR_INVALID) 
			{
				if (att.get_value_flag() == false)
				{
					TangoSys_OMemStream o;

					try
					{
						string att_name(names[i]);
						transform(att_name.begin(),att_name.end(),att_name.begin(),::tolower);

						vector<PollObj *>::iterator ite = get_polled_obj_by_type_name(Tango::POLL_ATTR,att_name);
						long upd = (*ite)->get_upd();
						if (upd == 0)
						{
							o << "Attribute ";
							o << att.get_name();
							o << " value is available only by CACHE.\n";
							o << "Attribute values are set by external polling buffer filling" << ends;
						}
						else
						{
							o << "Read value for attribute ";
							o << att.get_name();
							o << " has not been updated" << ends;
						}
					}
					catch (Tango::DevFailed &)
					{
						o << "Read value for attribute ";
						o << att.get_name();
						o << " has not been updated" << ends;
					}		
							
					(*back)[index].err_list.length(1);
					(*back)[index].err_list[0].severity = Tango::ERR;
					(*back)[index].err_list[0].reason = CORBA::string_dup("API_AttrValueNotSet");
					(*back)[index].err_list[0].origin = CORBA::string_dup("Device_3Impl::read_attributes_no_except");
#if ((defined _TG_WINDOWS_) || (defined __SUNPRO_CC) || (defined GCC_STD))
					string s = o.str();
					(*back)[index].err_list[0].desc = CORBA::string_dup(s.c_str());
#else
					char *tmp_str = o.str();
					(*back)[index].err_list[0].desc = CORBA::string_dup(tmp_str);
					delete[]tmp_str;
#endif
					(*back)[index].quality = Tango::ATTR_INVALID;
					(*back)[index].name = CORBA::string_dup(att.get_name().c_str());
					clear_att_dim((*back)[index]);
				}
				else
				{
					try
					{
						Tango::AttrWriteType w_type = att.get_writable();
						if ((w_type == Tango::READ) ||
			    		    	    (w_type == Tango::READ_WRITE) ||
			    		    	    (w_type == Tango::READ_WITH_WRITE))
						{
							if ((w_type == Tango::READ_WRITE) || (w_type == Tango::READ_WITH_WRITE))
								dev_attr->add_write_value(att);
									
							if ((att.is_alarmed().any() == true) && (qual != Tango::ATTR_INVALID))
								att.check_alarm();

						}
				
						CORBA::Any &a = (*back)[index].value;
						switch (att.get_data_type())
						{
						case Tango::DEV_SHORT :
						{
							Tango::DevVarShortArray *ptr = att.get_short_value();
							a <<= *ptr;
							delete ptr;
							break;
						}
				
						case Tango::DEV_LONG :
						{
							Tango::DevVarLongArray *ptr = att.get_long_value();
							a <<= *ptr;
							delete ptr;
							break;
						}
							
						case Tango::DEV_LONG64 :
						{
							Tango::DevVarLong64Array *ptr = att.get_long64_value();
							a <<= *ptr;
							delete ptr;
							break;
						}
				
						case Tango::DEV_DOUBLE :
						{
							Tango::DevVarDoubleArray *ptr = att.get_double_value();
							a <<= *ptr;
							delete ptr;
							break;
						}
				
						case Tango::DEV_STRING :
						{
							Tango::DevVarStringArray *ptr = att.get_string_value();
							a <<= *ptr;
							delete ptr;
							break;
						}
							
						case Tango::DEV_FLOAT :
						{
							Tango::DevVarFloatArray *ptr = att.get_float_value();
							a <<= *ptr;
							delete ptr;
							break;
						}
				
						case Tango::DEV_BOOLEAN :
						{
							Tango::DevVarBooleanArray *ptr = att.get_boolean_value();
							a <<= *ptr;
							delete ptr;
							break;
						}
				
						case Tango::DEV_USHORT :
						{
							Tango::DevVarUShortArray *ptr = att.get_ushort_value();
							a <<= *ptr;
							delete ptr;
							break;
						}
				
						case Tango::DEV_UCHAR :
						{
							Tango::DevVarUCharArray *ptr = att.get_uchar_value();
							a <<= *ptr;
							delete ptr;
							break;
						}
							
						case Tango::DEV_ULONG :
						{
							Tango::DevVarULongArray *ptr = att.get_ulong_value();
							a <<= *ptr;
							delete ptr;
							break;
						}
							
						case Tango::DEV_ULONG64 :
						{
							Tango::DevVarULong64Array *ptr = att.get_ulong64_value();
							a <<= *ptr;
							delete ptr;
							break;
						}
							
						case Tango::DEV_STATE :
						{
							Tango::DevVarStateArray *ptr = att.get_state_value();
							a <<= *ptr;
							delete ptr;
							break;
						}
						}
			
						if (att.get_when().tv_sec == 0)
							att.set_time();	
						(*back)[index].time = att.get_when();
						(*back)[index].quality = att.get_quality();
						(*back)[index].name = CORBA::string_dup(att.get_name().c_str());
						(*back)[index].r_dim.dim_x = att.get_x();
						(*back)[index].r_dim.dim_y = att.get_y();
						if ((w_type == Tango::READ_WRITE) ||
						    (w_type == Tango::READ_WITH_WRITE))
						{
							WAttribute &assoc_att = dev_attr->get_w_attr_by_ind(att.get_assoc_ind());
							(*back)[index].w_dim.dim_x = assoc_att.get_w_dim_x();
							(*back)[index].w_dim.dim_y = assoc_att.get_w_dim_y();
						}
						else
						{
							(*back)[index].w_dim.dim_x = 0;
							(*back)[index].w_dim.dim_y = 0;
						}
					}
					catch (Tango::DevFailed &e)
					{
						(*back)[index].err_list = e.errors;
						(*back)[index].quality = Tango::ATTR_INVALID;
						(*back)[index].name = CORBA::string_dup(att.get_name().c_str());
						clear_att_dim((*back)[index]);
					}
				}
			}
			else 
			{
				if (qual != Tango::ATTR_INVALID)
					qual = Tango::ATTR_INVALID;
				if (att.get_when().tv_sec == 0)
					att.set_time();
				(*back)[index].time = att.get_when();
				(*back)[index].quality = qual;
				(*back)[index].name = CORBA::string_dup(att.get_name().c_str());
				(*back)[index].r_dim.dim_x = 0;
				(*back)[index].r_dim.dim_y = 0;
				(*back)[index].w_dim.dim_x = 0;
				(*back)[index].w_dim.dim_y = 0;
			}
		}
	}
	
	cout4 << "Leaving Device_3Impl::read_attributes_no_except" << endl;
}


//+-------------------------------------------------------------------------
//
// method : 		Device_3Impl::read_attributes_from_cache
// 
// description : 	Read attributes from cache but do not throw exception
//			if it fails. This method is mainly a copy of the original
//			DeviceImpl::read_attributes method.
//
// argument: in :	- names: The names of the attribute to read
//			- back : Pointer to the sequence returned to the client.
//				 Memory has already been allocated for
//				 this pointer
//
//--------------------------------------------------------------------------


void Device_3Impl::read_attributes_from_cache(const Tango::DevVarStringArray& names,
					      Tango::AttributeValueList_3 *&back)
{
	unsigned long nb_names = names.length();
	cout4 << "Reading " << nb_names << " attr in read_attributes_from_cache()" << endl;
	
//
// Check that device supports the wanted attribute and that the attribute
// is polled. If some are non polled, store
// their index in the real_names sequence in a vector
//

	vector<long> non_polled;
	vector<PollObj *> &poll_list = get_poll_obj_list();
	unsigned long nb_poll = poll_list.size();
	unsigned long i,j;
	
	for (i = 0;i < nb_names;i++)
	{
		try
		{
			dev_attr->get_attr_ind_by_name(names[i]);
			for (j = 0;j < nb_poll;j++)
			{
#ifdef _TG_WINDOWS_
				if (_stricmp(poll_list[j]->get_name().c_str(),names[i]) == 0)
#else
				if (strcasecmp(poll_list[j]->get_name().c_str(),names[i]) == 0)
#endif
					break;
			}
			if (j == nb_poll)
			{
				non_polled.push_back(i);
			}		
		}
		catch (Tango::DevFailed &e)
		{
			(*back)[i].err_list = e.errors;
			(*back)[i].quality = Tango::ATTR_INVALID;
			(*back)[i].name = CORBA::string_dup(names[i]);
			clear_att_dim((*back)[i]);
		}		
	}
	
//
// If some attributes are not polled but their polling update period is defined,
// and the attribute is not in the device list of attr which should not be
// polled, start to poll them
//

	bool found;
	vector<long> poll_period;
	long not_polled_attr = 0;
	
	if (non_polled.size() != 0)
	{
		
//
// Check that it is possible to start polling for the non polled attribute
//

		for (i = 0;i < non_polled.size();i++)
		{
			Attribute &att = dev_attr->get_attr_by_name(names[non_polled[i]]);
			poll_period.push_back(att.get_polling_period());
			
			if (poll_period.back() == 0)
			{
				TangoSys_OMemStream o;
				o << "Attribute " << att.get_name() << " not polled" << ends;
				
					
				(*back)[non_polled[i]].err_list.length(1);
				(*back)[non_polled[i]].err_list[0].severity = Tango::ERR;
				(*back)[non_polled[i]].err_list[0].reason = CORBA::string_dup("API_AttrNotPolled");
				(*back)[non_polled[i]].err_list[0].origin = CORBA::string_dup("Device_3Impl::read_attributes_from_cache");
#if ((defined _TG_WINDOWS_) || (defined __SUNPRO_CC) || (defined GCC_STD))
				string s = o.str();
				(*back)[non_polled[i]].err_list[0].desc = CORBA::string_dup(s.c_str());
#else
				char *tmp_str = o.str();
				(*back)[non_polled[i]].err_list[0].desc = CORBA::string_dup(tmp_str);
				delete[]tmp_str;
#endif
				(*back)[non_polled[i]].quality = Tango::ATTR_INVALID;
				(*back)[non_polled[i]].name = CORBA::string_dup(att.get_name().c_str());
				clear_att_dim((*back)[i]);
				not_polled_attr++;
					
				continue;
			}
			else
			{
				found = false;
				vector<string> &napa = get_non_auto_polled_attr();
				for (j = 0;j < napa.size();j++)
				{
#ifdef _TG_WINDOWS_
					if (_stricmp(napa[j].c_str(),names[non_polled[i]]) == 0)
#else
					if (strcasecmp(napa[j].c_str(),names[non_polled[i]]) == 0)
#endif
						found = true;
				}
				
				if (found == true)
				{
					TangoSys_OMemStream o;
					o << "Attribute " << att.get_name() << " not polled" << ends;
					(*back)[non_polled[i]].err_list.length(1);
					(*back)[non_polled[i]].err_list[0].severity = Tango::ERR;
					(*back)[non_polled[i]].err_list[0].reason = CORBA::string_dup("API_AttrNotPolled");
					(*back)[non_polled[i]].err_list[0].origin = CORBA::string_dup("Device_3Impl::read_attributes_from_cache");
#if ((defined _TG_WINDOWS_) || (defined __SUNPRO_CC) || (defined GCC_STD))
					string s = o.str();
					(*back)[non_polled[i]].err_list[0].desc = CORBA::string_dup(s.c_str());
#else
					char *tmp_str = o.str();
					(*back)[non_polled[i]].err_list[0].desc = CORBA::string_dup(tmp_str);
					delete[]tmp_str;
#endif
					(*back)[non_polled[i]].quality = Tango::ATTR_INVALID;
					(*back)[non_polled[i]].name = CORBA::string_dup(att.get_name().c_str());
					clear_att_dim((*back)[i]);
					not_polled_attr++;
					
					continue;								
				}
			}
		}

//
// Leave method if number of attributes which should not be polled
// is equal to the requested attribute number
//

		if (not_polled_attr == nb_names)
			return;
			
//
// Start polling
//			
				
		Tango::Util *tg = Tango::Util::instance();
		DServer *adm_dev = tg->get_dserver_device();

		DevVarLongStringArray *send = new DevVarLongStringArray();
		send->lvalue.length(1);
		send->svalue.length(3);
		send->svalue[0] = device_name.c_str();
		send->svalue[1] = CORBA::string_dup("attribute");

		long start_polling_nb = 0;						
		for (i = 0;i < non_polled.size();i++)
		{
			if ((*back)[non_polled[i]].err_list.length() != 0)
				continue;
								
			send->lvalue[0] = poll_period[i];
			send->svalue[2] = names[non_polled[i]];

			try
			{
				get_poll_monitor().rel_monitor();									
				adm_dev->add_obj_polling(send,false);
				get_poll_monitor().get_monitor();
				start_polling_nb++;
			}
			catch (DevFailed &e)
			{
				get_poll_monitor().get_monitor();				
				(*back)[non_polled[i]].err_list = e.errors;
				(*back)[non_polled[i]].quality = Tango::ATTR_INVALID;
				(*back)[non_polled[i]].name = CORBA::string_dup(names[non_polled[i]]);
				clear_att_dim((*back)[i]);
				
				continue;
			}
		}

		delete send;
			
//
// Wait for first polling
//

		if (start_polling_nb != 0)
		{
#ifdef _TG_WINDOWS_
			get_poll_monitor().rel_monitor();
			Sleep((DWORD)500);
			get_poll_monitor().get_monitor();
#else
			struct timespec to_wait,inter;
			to_wait.tv_sec = 0;
			to_wait.tv_nsec = 500000000;
			get_poll_monitor().rel_monitor();				
			nanosleep(&to_wait,&inter);
			get_poll_monitor().get_monitor();				
#endif
		}

	}

//
// For each attribute, check that some data are available in cache and that they
// are not too old
//
	
	for (i = 0;i < nb_names;i++)
	{
	
		if ((*back)[i].err_list.length() != 0)
			continue;
					
		PollObj *polled_attr = NULL;
		unsigned long j;
		for (j = 0;j < poll_list.size();j++)
		{
#ifdef _TG_WINDOWS_
			if ((poll_list[j]->get_type() == Tango::POLL_ATTR) &&
	   	    	    (_stricmp(poll_list[j]->get_name().c_str(),names[i]) == 0))
#else
			if ((poll_list[j]->get_type() == Tango::POLL_ATTR) &&
		  	    (strcasecmp(poll_list[j]->get_name().c_str(),names[i]) == 0))
#endif
			{
				polled_attr = poll_list[j];
				break;
			}
		}
				
//
// Check that some data is available in cache
//

		if (polled_attr->is_ring_empty() == true)
		{
			TangoSys_OMemStream o;
			o << "No data available in cache for attribute " << names[i] << ends;
			
			(*back)[i].err_list.length(1);
			(*back)[i].err_list[0].severity = Tango::ERR;
			(*back)[i].err_list[0].reason = CORBA::string_dup("API_NoDataYet");
			(*back)[i].err_list[0].origin = CORBA::string_dup("Device_3Impl::read_attributes_from_cache");
#if ((defined _TG_WINDOWS_) || (defined __SUNPRO_CC) || (defined GCC_STD))
			string s = o.str();
			(*back)[i].err_list[0].desc = CORBA::string_dup(s.c_str());
#else
			char *tmp_str = o.str();
			(*back)[i].err_list[0].desc = CORBA::string_dup(tmp_str);
			delete[]tmp_str;
#endif
			(*back)[i].quality = Tango::ATTR_INVALID;
			(*back)[i].name = CORBA::string_dup(names[i]);
			clear_att_dim((*back)[i]);
					
			continue;								
		}
		
//
// Check that data are still refreshed by the polling thread
// Skip this test for object with external polling triggering (upd = 0)
//

		long tmp_upd = polled_attr->get_upd();
		if (tmp_upd != 0)
		{
			double last = polled_attr->get_last_insert_date();
			struct timeval now;
#ifdef _TG_WINDOWS_
			struct _timeb now_win;
			_ftime(&now_win);
			now.tv_sec = (unsigned long)now_win.time;
			now.tv_usec = (long)now_win.millitm * 1000;
#else
			gettimeofday(&now,NULL);
#endif
			now.tv_sec = now.tv_sec - DELTA_T;
			double now_d = (double)now.tv_sec + ((double)now.tv_usec / 1000000);
			double diff_d = now_d - last;
			if (diff_d > polled_attr->get_authorized_delta())
			{
				TangoSys_OMemStream o;
				o << "Data in cache for attribute " << names[i];
				o << " not updated any more" << ends;
			
				(*back)[i].err_list.length(1);
				(*back)[i].err_list[0].severity = Tango::ERR;
				(*back)[i].err_list[0].reason = CORBA::string_dup("API_NotUpdatedAnyMore");
				(*back)[i].err_list[0].origin = CORBA::string_dup("Device_3Impl::read_attributes_from_cache");
#if ((defined _TG_WINDOWS_) || (defined __SUNPRO_CC) || (defined GCC_STD))
				string s = o.str();
				(*back)[i].err_list[0].desc = CORBA::string_dup(s.c_str());
#else
				char *tmp_str = o.str();
				(*back)[i].err_list[0].desc = CORBA::string_dup(tmp_str);
				delete[]tmp_str;
#endif
				(*back)[i].quality = Tango::ATTR_INVALID;
				(*back)[i].name = CORBA::string_dup(names[i]);
				clear_att_dim((*back)[i]);
					
				continue;			
			}
		}

//
// Get attribute data type
//

		Attribute &att = dev_attr->get_attr_by_name(names[i]);
		long type = att.get_data_type();
		
//
// Finally, after all these checks, get value and store it in the sequence
// sent back to user
// In order to avoid unnecessary copy, don't use the assignement operator of the
// AttributeValue structure which copy each element and therefore also copy
// the Any object. The Any assignement operator is a deep copy!
// Create a new sequence using the attribute buffer and insert it into the
// Any. The sequence inside the source Any has been created using the attribute
// data buffer.
//

		try
		{
			{
				omni_mutex_lock sync(*polled_attr);
				
				AttributeValue_3 &att_val = polled_attr->get_last_attr_value_3(false);
				if (att_val.quality != Tango::ATTR_INVALID)
				{					
					const Tango::DevVarDoubleArray *tmp_db;
					Tango::DevVarDoubleArray *new_tmp_db;
					const Tango::DevVarShortArray *tmp_sh;
					Tango::DevVarShortArray *new_tmp_sh;
					const Tango::DevVarLongArray *tmp_lg;
					Tango::DevVarLongArray *new_tmp_lg;
					const Tango::DevVarLong64Array *tmp_lg64;
					Tango::DevVarLong64Array *new_tmp_lg64;
					const Tango::DevVarStringArray *tmp_str;
					Tango::DevVarStringArray *new_tmp_str;
					const Tango::DevVarFloatArray *tmp_fl;
					Tango::DevVarFloatArray *new_tmp_fl;
					const Tango::DevVarBooleanArray *tmp_boo;
					Tango::DevVarBooleanArray *new_tmp_boo;
					const Tango::DevVarUShortArray *tmp_ush;
					Tango::DevVarUShortArray *new_tmp_ush;
					const Tango::DevVarCharArray *tmp_uch;
					Tango::DevVarCharArray *new_tmp_uch;
					const Tango::DevVarULongArray *tmp_ulg;
					Tango::DevVarULongArray *new_tmp_ulg;
					const Tango::DevVarULong64Array *tmp_ulg64;
					Tango::DevVarULong64Array *new_tmp_ulg64;
					const Tango::DevVarStateArray *tmp_state;
					Tango::DevVarStateArray *new_tmp_state;
					Tango::DevState sta;
																														
					switch (type)
					{
					case Tango::DEV_SHORT :
						att_val.value >>= tmp_sh;
						new_tmp_sh = new DevVarShortArray(tmp_sh->length(),
										  tmp_sh->length(),
										  const_cast<short *>(tmp_sh->get_buffer()),
										  false);
						(*back)[i].value <<= new_tmp_sh;
						break;

					case Tango::DEV_DOUBLE :					
						att_val.value >>= tmp_db;		
						new_tmp_db = new DevVarDoubleArray(tmp_db->length(),
										   tmp_db->length(),
										   const_cast<double *>(tmp_db->get_buffer()),
										   false);
						(*back)[i].value <<= new_tmp_db;
						break;

					case Tango::DEV_LONG :
						att_val.value >>= tmp_lg;
						new_tmp_lg = new DevVarLongArray(tmp_lg->length(),tmp_lg->length(),
										 const_cast<DevLong *>(tmp_lg->get_buffer()),false);
						(*back)[i].value <<= new_tmp_lg;
						break;
						
					case Tango::DEV_LONG64 :
						att_val.value >>= tmp_lg64;
						new_tmp_lg64 = new DevVarLong64Array(tmp_lg64->length(),tmp_lg64->length(),
										 const_cast<DevLong64 *>(tmp_lg64->get_buffer()),false);
						(*back)[i].value <<= new_tmp_lg64;
						break;

					case Tango::DEV_STRING :
						att_val.value >>= tmp_str;
						new_tmp_str = new DevVarStringArray(tmp_str->length(),
										    tmp_str->length(),
										    const_cast<char **>(tmp_str->get_buffer()),
										    false);
						(*back)[i].value <<= new_tmp_str;
						break;
						
					case Tango::DEV_FLOAT :
						att_val.value >>= tmp_fl;
						new_tmp_fl = new DevVarFloatArray(tmp_fl->length(),
										  tmp_fl->length(),
										  const_cast<float *>(tmp_fl->get_buffer()),
										  false);
						(*back)[i].value <<= new_tmp_fl;
						break;

					case Tango::DEV_BOOLEAN :					
						att_val.value >>= tmp_boo;		
						new_tmp_boo = new DevVarBooleanArray(tmp_boo->length(),
										   tmp_boo->length(),
										   const_cast<DevBoolean *>(tmp_boo->get_buffer()),
										   false);
						(*back)[i].value <<= new_tmp_boo;
						break;

					case Tango::DEV_USHORT :
						att_val.value >>= tmp_ush;
						new_tmp_ush = new DevVarUShortArray(tmp_ush->length(),
										 tmp_ush->length(),
										 const_cast<DevUShort *>(tmp_ush->get_buffer()),
										 false);
						(*back)[i].value <<= new_tmp_ush;
						break;

					case Tango::DEV_UCHAR :
						att_val.value >>= tmp_uch;
						new_tmp_uch = new DevVarCharArray(tmp_uch->length(),
										    tmp_uch->length(),
										    const_cast<DevUChar *>(tmp_uch->get_buffer()),
										    false);
						(*back)[i].value <<= new_tmp_uch;
						break;
						
					case Tango::DEV_ULONG :
						att_val.value >>= tmp_ulg;
						new_tmp_ulg = new DevVarULongArray(tmp_ulg->length(),tmp_ulg->length(),
										 const_cast<DevULong *>(tmp_ulg->get_buffer()),false);
						(*back)[i].value <<= new_tmp_ulg;
						break;
						
					case Tango::DEV_ULONG64 :
						att_val.value >>= tmp_ulg64;
						new_tmp_ulg64 = new DevVarULong64Array(tmp_ulg64->length(),tmp_ulg64->length(),
										 const_cast<DevULong64 *>(tmp_ulg64->get_buffer()),false);
						(*back)[i].value <<= new_tmp_ulg64;
						break;
						
					case Tango::DEV_STATE :
						CORBA::TypeCode_var ty;
						ty = att_val.value.type();
			
						if (ty->kind() == CORBA::tk_enum)
						{
							att_val.value >>= sta;
							(*back)[i].value <<= sta;
						}
						else
						{
							att_val.value >>= tmp_state;
							new_tmp_state = new DevVarStateArray(tmp_state->length(),tmp_state->length(),
											 const_cast<DevState *>(tmp_state->get_buffer()),false);
							(*back)[i].value <<= new_tmp_state;
						}
						break;
					}
				}
				(*back)[i].quality= att_val.quality;
				(*back)[i].time = att_val.time;
				(*back)[i].r_dim = att_val.r_dim;
				(*back)[i].w_dim = att_val.w_dim;
				(*back)[i].name = CORBA::string_dup(att_val.name);
			}
		}
		catch (Tango::DevFailed &e)
		{
			(*back)[i].err_list = e.errors;
			(*back)[i].quality = Tango::ATTR_INVALID;
			(*back)[i].name = CORBA::string_dup(names[i]);
			clear_att_dim((*back)[i]);
		}		
	}
}


//+-------------------------------------------------------------------------
//
// method : 		DeviceImpl::write_attributes_3
// 
// description : 	CORBA operation to write attribute(s) value
//
// argument: in :	- values: The new attribute(s) value to be set.
//
//--------------------------------------------------------------------------

void Device_3Impl::write_attributes_3(const Tango::AttributeValueList& values)
throw (Tango::MultiDevFailed, Tango::DevFailed, CORBA::SystemException)
{
	AutoTangoMonitor sync(this,true);
	cout4 << "Device_3Impl::write_attributes_3 arrived" << endl;

//
// Record operation request in black box
//

	blackbox_ptr->insert_attr(values,3);

//
// Return exception if the device does not have any attribute
//

	unsigned long nb_dev_attr = dev_attr->get_attr_nb();
	if (nb_dev_attr == 0)
	{
		Except::throw_exception((const char *)"API_AttrNotFound",
				        (const char *)"The device does not have any attribute",
				        (const char *)"DeviceImpl::write_attributes");
	}

	unsigned long nb_failed = 0;
	Tango::NamedDevErrorList errs;
			
//
// Retrieve index of wanted attributes in the device attribute list
//

	vector<AttIdx> updated_attr;
	unsigned long nb_updated_attr = values.length();

	unsigned long i;
	for (i = 0;i < nb_updated_attr;i++)
	{
		try
		{
			AttIdx idxs;
			idxs.idx_in_names = i;
			idxs.idx_in_multi_attr = dev_attr->get_attr_ind_by_name(values[i].name);
			updated_attr.push_back(idxs);

//
// Check that these attributes are writable.
// For attributes which are not scalar, also check that
// their dimensions are correct
//

			if ((dev_attr->get_attr_by_ind(updated_attr.back().idx_in_multi_attr).get_writable() == Tango::READ) ||
		    	    (dev_attr->get_attr_by_ind(updated_attr.back().idx_in_multi_attr).get_writable() == Tango::READ_WITH_WRITE))
			{
				TangoSys_OMemStream o;
		
				o << "Attribute ";
				o << dev_attr->get_attr_by_ind(updated_attr.back().idx_in_multi_attr).get_name();
				o << " is not writable" << ends;

				updated_attr.pop_back();
				Except::throw_exception((const char *)"API_AttrNotWritable",
					        o.str(),
					        (const char *)"DeviceImpl::write_attributes");
			}

			Attribute &att = dev_attr->get_attr_by_ind(updated_attr.back().idx_in_multi_attr);
			if (att.get_data_format() != Tango::SCALAR)
			{
				TangoSys_OMemStream o;
				bool err = false;			
			
				if (att.get_max_dim_x() < values[i].dim_x)
				{
					err = true;		
					o << "X ";
				}

				if (err == false)
				{			
					if (att.get_max_dim_y() < values[i].dim_y)
					{
						err = true;
						o << "Y ";
					}
				}
			
				if (err == true)
				{		
					o << "dimesion is greater than the max defined for attribute ";
					o << att.get_name();
					o << ends;

					updated_attr.pop_back();
					Except::throw_exception((const char *)"API_WAttrOutsideLimit",
					        o.str(),
					        (const char *)"DeviceImpl::write_attributes");
				}
			}
		}
		catch (Tango::DevFailed &e)
		{
			nb_failed++;
			errs.length(nb_failed);
			errs[nb_failed - 1].name = CORBA::string_dup(values[i].name);
			errs[nb_failed - 1].index_in_call = i;
			errs[nb_failed - 1].err_list = e.errors;
		}
	}

//
// Call the always_executed_hook
//

	if (nb_failed != nb_updated_attr)
		always_executed_hook();
	
//
// Set attribute internal value
//

	vector<AttIdx>::iterator ctr;
	for (ctr = updated_attr.begin();ctr < updated_attr.end();++ctr)
	{
		try
		{
			dev_attr->get_w_attr_by_ind(ctr->idx_in_multi_attr).set_write_value(values[ctr->idx_in_names].value,
								 	  (unsigned long)values[ctr->idx_in_names].dim_x,
									  (unsigned long)values[ctr->idx_in_names].dim_y);
		}
		catch (Tango::DevFailed &e)
		{
			nb_failed++;
			errs.length(nb_failed);
			errs[nb_failed - 1].name = CORBA::string_dup(values[ctr->idx_in_names].name);
			errs[nb_failed - 1].index_in_call = ctr->idx_in_names;
			errs[nb_failed - 1].err_list = e.errors;
			ctr = updated_attr.erase(ctr);
			if (ctr >= updated_attr.end())
				break;
			else
			{
				if (ctr == updated_attr.begin())
					break;
				else
					--ctr;
			}
		}
	}
	
//
// Write the hardware. Call this method one attribute at a time in order to
// correctly initialized the MultiDevFailed exception in case one of the
// attribute failed.
//

//	vector<long> one_att(1);
	if (nb_failed != nb_updated_attr)
	{
		vector<AttIdx>::iterator ite;
		for(ite = updated_attr.begin();ite != updated_attr.end();++ite)
		{
//			one_att[0] = updated_attr[i].idx_in_multi_attr;
			try
			{
				WAttribute &att = dev_attr->get_w_attr_by_ind((*ite).idx_in_multi_attr);
				att.set_value_flag(false);
				vector<Tango::Attr *> &attr_vect = device_class->get_class_attr()->get_attr_list();
				if (attr_vect[att.get_attr_idx()]->is_allowed(this,Tango::WRITE_REQ) == false)
				{
					TangoSys_OMemStream o;
		
					o << "It is actually not allowed to write attribute ";
					o << att.get_name();
					o << ". The device state is " << Tango::DevStateName[get_state()] << ends;


					Except::throw_exception((const char *)"API_AttrNotAllowed",
					        		o.str(),
					        		(const char *)"Device_3Impl::write_attributes");
				}
				attr_vect[att.get_attr_idx()]->write(this,att);
//				write_attr_hardware(one_att);
			}
			catch (Tango::DevFailed &e)
			{
				nb_failed++;
				errs.length(nb_failed);
				errs[nb_failed - 1].name = CORBA::string_dup(values[(*ite).idx_in_names].name);
				errs[nb_failed - 1].index_in_call = (*ite).idx_in_names;
				errs[nb_failed - 1].err_list = e.errors;
				ite = updated_attr.erase(ite);
				if (ite >= updated_attr.end())
					break;
				else
				{
					if (ite == updated_attr.begin())
						break;
					else
						--ite;
				}		
			}
		}
	}
		
//
// Copy data into Attribute object, store the memorized one in db
// and if the attribute has a RDS alarm, set the write date
//

	vector<long> att_in_db;
	
	for (i = 0;i < updated_attr.size();i++)
	{
		WAttribute &att = dev_attr->get_w_attr_by_ind(updated_attr[i].idx_in_multi_attr);

		att.copy_data(values[updated_attr[i].idx_in_names].value);
		if (att.is_memorized() == true)
			att_in_db.push_back(i);
		if (att.is_alarmed().test(Attribute::rds) == true)
			att.set_written_date();			
	
	}
	
	if ((Tango::Util::_UseDb == true) && (att_in_db.size() != 0))
	{
		try
		{
			write_attributes_in_db(att_in_db,updated_attr);
		}
		catch (Tango::DevFailed &e)
		{
			errs.length(nb_failed + att_in_db.size());
			for (i = 0;i < att_in_db.size();i++)
			{
				errs[nb_failed + i].name = CORBA::string_dup(values[updated_attr[att_in_db[i]].idx_in_names].name);
				errs[nb_failed + i].index_in_call = updated_attr[att_in_db[i]].idx_in_names;
				errs[nb_failed + i].err_list = e.errors;
			}
			nb_failed = nb_failed + att_in_db.size();
		}
	}
		
//
// Return to caller.
//

	cout4 << "Leaving Device_3Impl::write_attributes" << endl;
		
	if (nb_failed != 0)
	{
		throw Tango::MultiDevFailed(errs);
	}
		
}


//+-------------------------------------------------------------------------
//
// method : 		Device_3Impl::read_attribute_history_3
// 
// description : 	CORBA operation to read attribute alue history from 
//			the polling buffer.
//
// argument: in :	- name : attribute name
//			- n : history depth (in record number)
//
// This method returns a pointer to a DevAttrHistoryList with one 
// DevAttrHistory structure for each attribute record
//
//--------------------------------------------------------------------------

Tango::DevAttrHistoryList_3 *Device_3Impl::read_attribute_history_3(const char* name,
								    CORBA::Long n)
throw(Tango::DevFailed, CORBA::SystemException)
{
	TangoMonitor &mon = get_poll_monitor();
	AutoTangoMonitor sync(&mon);
	
	cout4 << "Device_3Impl::read_attribute_history_3 arrived" << endl;

//
// Record operation request in black box
//

	blackbox_ptr->insert_op(Op_Read_Attr_history_3);
	
	Tango::DevAttrHistoryList_3 *back;
	vector<PollObj *> &poll_list = get_poll_obj_list();
	long nb_poll = poll_list.size();
	
//
// Check that the device supports this attribute. This method returns an 
// exception in case of unsupported attribute
//

	Attribute &att = dev_attr->get_attr_by_name(name);

	string attr_str(name);
	transform(attr_str.begin(),attr_str.end(),attr_str.begin(),::tolower);
	
//
// Check that the wanted attribute is polled.
//

	long j;
	PollObj *polled_attr = NULL;
	for (j = 0;j < nb_poll;j++)
	{
		if ((poll_list[j]->get_type() == Tango::POLL_ATTR) &&
		    (poll_list[j]->get_name() == attr_str))
		{
			polled_attr = poll_list[j];
			break;
		}
	}
	if (polled_attr == NULL)
	{
		TangoSys_OMemStream o;
		o << "Attribute " << attr_str << " not polled" << ends;
		Except::throw_exception((const char *)"API_AttrNotPolled",
					o.str(),
					(const char *)"Device_3Impl::read_attribute_history_3");
	}

//
// Check that some data is available in cache
//

	if (polled_attr->is_ring_empty() == true)
	{
		TangoSys_OMemStream o;
		o << "No data available in cache for attribute " << attr_str << ends;
		Except::throw_exception((const char *)"API_NoDataYet",
					o.str(),
					(const char *)"Device_3Impl::read_attribute_history_3");
	}

//
// Set the number of returned records
//

	long in_buf = polled_attr->get_elt_nb_in_buffer();
	if (n > in_buf)
		n = in_buf;

//
// Allocate memory for the returned value
//
		
	try
	{
		back = new Tango::DevAttrHistoryList_3(n);
                back->length(n);
	}	
	catch (bad_alloc)
	{
		Except::throw_exception((const char *)"API_MemoryAllocation",
				        (const char *)"Can't allocate memory in server",
				        (const char *)"Device_3Impl::read_attribute_history_3");
	}

//
// Get attribute value history
//

	polled_attr->get_attr_history(n,back,att.get_data_type());

	cout4 << "Leaving Device_3Impl::command_inout_history_3 method" << endl;				
	return back;
}


//+-------------------------------------------------------------------------
//
// method :		Device_3Impl::info_3
// 
// description :	CORBA operation to get device info
//
//--------------------------------------------------------------------------


Tango::DevInfo_3 *Device_3Impl::info_3()
throw (Tango::DevFailed, CORBA::SystemException)
{
	cout4 << "Device_3Impl::info_3 arrived" << endl;

	Tango::DevInfo_3 *back;

//
// Allocate memory for the stucture sent back to caller. The ORB will free it
//

	try
	{
		back = new Tango::DevInfo_3();
	}	
	catch (bad_alloc)
	{
		Except::throw_exception((const char *)"API_MemoryAllocation",
				      (const char *)"Can't allocate memory in server",
				      (const char *)"Device_3Impl::info_3");
	}

//
// Retrieve server host
//

	Tango::Util *tango_ptr = Tango::Util::instance();
	back->server_host = CORBA::string_dup(tango_ptr->get_host_name().c_str());
		
//
// Fill-in remaining structure fields
//

	back->dev_class = CORBA::string_dup(device_class->get_name().c_str());
	back->server_id = CORBA::string_dup(tango_ptr->get_ds_name().c_str());
	back->server_version = DevVersion;
	back->doc_url = CORBA::string_dup(device_class->get_doc_url().c_str());
	back->dev_type = CORBA::string_dup(device_class->get_type().c_str());
		
//
// Record operation request in black box
//

	blackbox_ptr->insert_op(Op_Info_3);
	
//
// Return to caller
//
				
	cout4 << "Leaving Device_3Impl::info_3" << endl;
	return back;
}


//+-------------------------------------------------------------------------
//
// method : 		Device_3Impl::get_attribute_config_3
// 
// description : 	CORBA operation to get attribute configuration.
//
// argument: in :	- names: name of attribute(s)
//
// This method returns a pointer to a AttributeConfigList_3 with one 
// AttributeConfig_3 structure for each atribute
//
//
// WARNING !!!!!!!!!!!!!!!!!!
//
// This is the release 3 of this CORBA operation which
// returns much more parameter than in release 2
// The code has been duplicated in order to keep it clean
// (avoid many "if" on version number in a common method)
//
//--------------------------------------------------------------------------

Tango::AttributeConfigList_3 *Device_3Impl::get_attribute_config_3(const Tango::DevVarStringArray& names)
throw(Tango::DevFailed, CORBA::SystemException)
{
	TangoMonitor &mon = get_att_conf_monitor();
	AutoTangoMonitor sync(&mon);

	cout4 << "Device_3Impl::get_attribute_config_3 arrived" << endl;

	long nb_attr = names.length();
	Tango::AttributeConfigList_3 *back;
	bool all_attr = false;

//
// Record operation request in black box
//

	blackbox_ptr->insert_op(Op_Get_Attr_Config_3);
	
//
// Get attribute number and device version
//

	long nb_dev_attr = dev_attr->get_attr_nb();
	
//
// Check if the caller want to get config for all attribute
// If the device implements IDL 3 (State and status as attributes)
// and the client is an old one (not able to read state/status as
// attribute), decrement attribute number
//

	string in_name(names[0]);
	if (nb_attr == 1)
	{
		if (in_name == AllAttr_3)
		{
			all_attr = true;
			nb_attr = nb_dev_attr;
		}	
	}
		
//
// Allocate memory for the AttributeConfig structures
//

	try
	{
		back = new Tango::AttributeConfigList_3(nb_attr);
		back->length(nb_attr);
	}
	catch (bad_alloc)
	{
		Except::throw_exception((const char *)"API_MemoryAllocation",
				        (const char *)"Can't allocate memory in server",
				        (const char *)"Device_3Impl::get_attribute_config_3");
	}

//
// Fill in these structures
//
		
	for (long i = 0;i < nb_attr;i++)
	{
		try
		{
			if (all_attr == true)
			{
				Attribute &attr = dev_attr->get_attr_by_ind(i);
				attr.get_properties_3((*back)[i]);
			}
			else
			{
				Attribute &attr = dev_attr->get_attr_by_name(names[i]);
				attr.get_properties_3((*back)[i]);
			}
		}
		catch (Tango::DevFailed e)
		{
			delete back;
			throw;
		}
	}
		
//
// Return to caller
//

	cout4 << "Leaving Device_3Impl::get_attribute_config_3" << endl;
	
	return back;
}

//+-------------------------------------------------------------------------
//
// method :		Device_3Impl::set_attribute_config_3
// 
// description :	CORBA operation to set attribute configuration locally
//			and in the Tango database
//
// argument: in :	- new_conf: The new attribute(s) configuration. One
//				    AttributeConfig structure is needed for each
//				    attribute to update
//
//--------------------------------------------------------------------------

void Device_3Impl::set_attribute_config_3(const Tango::AttributeConfigList_3& new_conf)
throw (Tango::DevFailed, CORBA::SystemException)
{
	AutoTangoMonitor sync(this,true);
	cout4 << "DeviceImpl::set_attribute_config_3 arrived" << endl;

//
// The attribute conf. is protected by two monitors. One protects access between 
// get and set attribute conf. The second one protects access between set and
// usage. This is the classical device monitor
//

	TangoMonitor &mon1 = get_att_conf_monitor();
	AutoTangoMonitor sync1(&mon1);
	
//
// Record operation request in black box
//

	blackbox_ptr->insert_op(Op_Set_Attr_Config_3);

//
// Return exception if the device does not have any attribute
//

	long nb_dev_attr = dev_attr->get_attr_nb();
	if (nb_dev_attr == 0)
	{
		Except::throw_exception((const char *)"API_AttrNotFound",
				        (const char *)"The device does not have any attribute",
				        (const char *)"Device_3Impl::set_attribute_config_3");
	}
	
//
// Get some event related data
//
	
	Tango::Util *tg = Tango::Util::instance();
	EventSupplier *ev_supply = tg->get_event_supplier();
	
//
// Update attribute config first in database, then locally
//
	
	long nb_attr = new_conf.length();
	long i;

	try
	{	
		for (i = 0;i < nb_attr;i++)
		{
			string tmp_name(new_conf[i].name);
			transform(tmp_name.begin(),tmp_name.end(),tmp_name.begin(),::tolower);
			if ((tmp_name == "state") || (tmp_name == "status"))
			{
				Except::throw_exception((const char *)"API_AttrNotFound",
				        		(const char *)"Cannot set config for attribute state or status",
				        		(const char *)"Device_3Impl::set_attribute_config_3");
			}

			Attribute &attr = dev_attr->get_attr_by_name(new_conf[i].name);
			bool old_alarm = attr.is_alarmed().any();
			if (Tango::Util::_UseDb == true)
				attr.upd_database(new_conf[i],device_name);
			attr.set_properties(new_conf[i],device_name);
			
//
// In case the attribute quality factor was set to ALARM, reset it to VALID
//

			if ((attr.get_quality() == Tango::ATTR_ALARM) &&
			    (old_alarm == true) &&
			    (attr.is_alarmed().any() == false))
				attr.set_quality(Tango::ATTR_VALID);
				
//
// Send the event
//
				
			if (ev_supply != NULL)
			{
				string tmp_name(new_conf[i].name);
				ev_supply->push_att_conf_events(this,new_conf[i],(Tango::DevFailed *)NULL,tmp_name);
			}
		}

	}
	catch (Tango::DevFailed &e)
	{
	
//
// Re build the list of "alarmable" attribute
//

		dev_attr->get_alarm_list().clear();
		for (long j = 0;j < nb_dev_attr;j++)
		{
			Attribute &att = dev_attr->get_attr_by_ind(j);
			if (att.is_alarmed().any() == true)
			{
				if (att.get_writable() != Tango::WRITE)
					dev_attr->get_alarm_list().push_back(j);
			}
		}		
	
//
// Change the exception reason flag
//

		TangoSys_OMemStream o;
		
		o << e.errors[0].reason;
		if (i != 0)
			o << "\nAll previous attribute(s) have been successfully updated";
		if (i != (nb_attr - 1))
			o << "\nAll remaining attribute(s) have not been updated";
		o << ends;

#if ((defined _TG_WINDOWS_) || (defined __SUNPRO_CC) || (defined GCC_STD))
		string s = o.str();
		e.errors[0].reason = CORBA::string_dup(s.c_str());
#else		
		char *mess = o.str();			
		e.errors[0].reason = CORBA::string_dup(mess);
		delete [] mess;
#endif
		throw;
	}

//
// Re build the list of "alarmable" attribute
//

	dev_attr->get_alarm_list().clear();
	for (i = 0;i < nb_dev_attr;i++)
	{
		Tango::Attribute &attr = dev_attr->get_attr_by_ind(i);
		Tango::AttrWriteType w_type = attr.get_writable();
		if (attr.is_alarmed().any() == true)
		{
			if (w_type != Tango::WRITE)
				dev_attr->get_alarm_list().push_back(i);
		}
	}
	
//
// Return to caller
//

	cout4 << "Leaving Device_3Impl::set_attribute_config_3" << endl;	
}


//+-------------------------------------------------------------------------
//
// method : 		Device_3Impl::write_attributes_in_db
// 
// description : 	Method to write memorized attributes in database
//
// argument: in :	- name : attribute name
//			- n : history depth (in record number)
//
// This method returns a pointer to a DevAttrHistoryList with one 
// DevAttrHistory structure for each attribute record
//
//--------------------------------------------------------------------------

void Device_3Impl::write_attributes_in_db(vector<long> &att_in_db,vector<AttIdx> &updated_attr)
{

//
// Store memorized attribute in db
//
	
	Tango::Util *tg = Tango::Util::instance();
	Tango::Database *db = tg->get_database();
		
	Tango::DbData db_data;
		
	for (unsigned long i = 0;i < att_in_db.size();i++)
	{
		Tango::DbDatum tmp_db;

//
// Update one property
//

		long idx = att_in_db[i];
		WAttribute &att = dev_attr->get_w_attr_by_ind(updated_attr[idx].idx_in_multi_attr);
		tmp_db.name = att.get_name();
		tmp_db << (short)1;		
		db_data.push_back(tmp_db);

//
// Init property value
//
			
		tmp_db.name = MemAttrPropName;
		const char *ptr;
		switch (att.get_data_type())
		{
		case Tango::DEV_SHORT :
			tmp_db << (*att.get_last_written_sh())[0];
			break;

		case Tango::DEV_LONG :
			tmp_db << (*att.get_last_written_lg())[0];
			break;
			
		case Tango::DEV_LONG64 :
			tmp_db << (*att.get_last_written_lg64())[0];
			break;
			
		case Tango::DEV_DOUBLE :
			tmp_db << (*att.get_last_written_db())[0];
			break;
			
		case Tango::DEV_STRING :
			ptr = (*att.get_last_written_str())[0].in();
			tmp_db << ptr;
			break;
			
		case Tango::DEV_FLOAT :
			tmp_db << (*att.get_last_written_fl())[0];
			break;

		case Tango::DEV_BOOLEAN :
			tmp_db << (*att.get_last_written_boo())[0];
			break;
			
		case Tango::DEV_USHORT :
			tmp_db << (*att.get_last_written_ush())[0];
			break;
			
		case Tango::DEV_UCHAR :
			tmp_db << (*att.get_last_written_uch())[0];
			break;
			
		case Tango::DEV_ULONG :
			tmp_db << (*att.get_last_written_ulg())[0];
			break;
			
		case Tango::DEV_ULONG64 :
			tmp_db << (*att.get_last_written_ulg64())[0];
			break;		
			
		case Tango::DEV_STATE :
			{
			Tango::DevState tmp_state = (*att.get_last_written_state())[0];
			tmp_db << (short)tmp_state;
			}
			break;
		}
		db_data.push_back(tmp_db);			
	}
		
	db->put_device_attribute_property(device_name,db_data);
		
}

void Device_3Impl::write_attributes_in_db(vector<long> &att_in_db,vector<long> &updated_attr)
{
	vector<AttIdx> v;
	for (unsigned int i = 0;i < updated_attr.size();i++)
	{
		AttIdx ai;
		ai.idx_in_multi_attr = updated_attr[i];
		v.push_back(ai);
	}
	
	write_attributes_in_db(att_in_db,v);
}

//+-------------------------------------------------------------------------
//
// method : 		Device_3Impl::add_state_status_attrs
// 
// description : 	Add state and status in the device attribute list
//
//--------------------------------------------------------------------------

void Device_3Impl::add_state_status_attrs()
{

//
// First, create the State attribute with default properties
//

	Tango::Attr att_state("State",Tango::DEV_STATE);
	vector<AttrProperty> prop_list_state;
	string att_name("State");
	dev_attr->add_default(prop_list_state,device_name,att_name);
	
	dev_attr->add_attr(new Attribute(prop_list_state,att_state,device_name,-1));
	
//
// Now, create the status attribute also with default properties
//

	Tango::Attr att_status("Status",Tango::DEV_STRING);
	vector<AttrProperty> prop_list_status;
	att_name = "Status";
	dev_attr->add_default(prop_list_status,device_name,att_name);
	
	dev_attr->add_attr(new Attribute(prop_list_status,att_status,device_name,-1));
	
}


//+-------------------------------------------------------------------------
//
// method : 		Device_3Impl::add_alarmed
// 
// description : 	Method to add alarmed attributes (if not already
//			there) in the attribute list passed as argument
//
// argument: in :	- att_list : The attribute index in the 
//				     multi attribute instance
//
//--------------------------------------------------------------------------

void Device_3Impl::add_alarmed(vector<long> &att_list)
{
	vector<long> &alarmed_list = dev_attr->get_alarm_list();
	long nb_wanted_attr = alarmed_list.size();

	if (nb_wanted_attr != 0)
	{
		for (int i = 0;i < nb_wanted_attr;i++)
		{
			long nb_attr = att_list.size();
			bool found = false;
			
			for (int j = 0;j < nb_attr;j++)
			{
				if (att_list[j] == alarmed_list[i])
				{
					found = true;
					break;
				}
			}

//
// If not found, add it
//
			
			if (found == false)
			{
				att_list.push_back(alarmed_list[i]);
			}
		}
	}
}


//+-------------------------------------------------------------------------
//
// method : 		Device_3Impl::reading_state_necessary
// 
// description : 	Method to check ifi t is necessary to read
//			state. If the device has some alarmed
//			attribute and one of these attributes
//			has already been read and failed, it is
//			not necessary to read state. It will also
//			failed.
//
// argument: in :	- wanted_attr : The list of attribute to
//					be read by this call
//
// This  method returns -1 if reading state is possible.
// Otherwise, it returns the index in the wanted_attr list
// of the alarmed attribute which failed
//
//--------------------------------------------------------------------------

long Device_3Impl::reading_state_necessary(vector<AttIdx> &wanted_attr)
{
	vector<long> &alarmed_list = dev_attr->get_alarm_list();
	long nb_alarmed_attr = alarmed_list.size();
	long ret = -1;
	
	if (nb_alarmed_attr == 0)
		ret = -1;
		
	else
	{
		long nb_attr = wanted_attr.size();
		for (int j = 0;j < nb_alarmed_attr;j++)
		{
			for (int i = 0;i < nb_attr;i++)
			{
				if (alarmed_list[j] == wanted_attr[i].idx_in_multi_attr)
				{
					if (wanted_attr[i].failed == true)
						return i;
				}
			}
		} 
	}
	
	return ret;
}

//+-------------------------------------------------------------------------
//
// method : 		Device_3Impl::alarmed_not_read
// 
// description : 	This method find all the attributes which
//			will be read by the state (because alarmed)
//			and which have been already read.
//			It builds a vector with the list of attribute
//			not read
//
// argument: in :	- wanted_attr : The list of attribute to
//					be read by this call
//
//--------------------------------------------------------------------------

void Device_3Impl::alarmed_not_read(vector<AttIdx> &wanted_attr)
{
	vector<long> &alarmed_list = dev_attr->get_alarm_list();
	long nb_alarmed_attr = alarmed_list.size();
	long nb_attr = wanted_attr.size();
	ext->alarmed_not_read.clear();
		
	for (int i = 0;i < nb_alarmed_attr;i++)
	{
		bool found = false;
		for (int j = 0;j < nb_attr;j++)
		{
			if (alarmed_list[i] == wanted_attr[j].idx_in_multi_attr)
			{
				found = true;
				break;
			}
		}
		
		if (found == false)
		{
			ext->alarmed_not_read.push_back(alarmed_list[i]);
		}		
	}
}


//+-------------------------------------------------------------------------
//
// method : 		Device_3Impl::state2attr
// 
// description : 	Method to send a device state as an attribute object
//
// argument: in :	- state : The device state
//			- back : The AttributeValue structure
//
//--------------------------------------------------------------------------

void Device_3Impl::state2attr(Tango::DevState state,Tango::AttributeValue_3 &back)
{
	back.value <<= state;
#ifdef _TG_WINDOWS_
	struct _timeb after_win;

	_ftime(&after_win);
	back.time.tv_sec = (long)after_win.time;
	back.time.tv_usec = (long)after_win.millitm * 1000;
	back.time.tv_nsec = 0;
#else
	struct timeval after;

	gettimeofday(&after,NULL);
	back.time.tv_sec = after.tv_sec;
	back.time.tv_usec = after.tv_usec;
	back.time.tv_nsec = 0;
#endif
	back.quality = Tango::ATTR_VALID;
	back.name = CORBA::string_dup("State");
	back.r_dim.dim_x = 1;
	back.r_dim.dim_y = 0;
	back.w_dim.dim_x = 0;
	back.w_dim.dim_y = 0;
}

//+-------------------------------------------------------------------------
//
// method : 		Device_3Impl::status2attr
// 
// description : 	Method to send a device status string as an
//			attribute object
//
// argument: in :	- status : The device status
//			- back : The AttributeValue structure
//
//--------------------------------------------------------------------------

void Device_3Impl::status2attr(Tango::ConstDevString status,Tango::AttributeValue_3 &back)
{
	Tango::DevVarStringArray str_seq(1);
	str_seq.length(1);
	str_seq[0] = CORBA::string_dup(status);
		
	back.value <<= str_seq;
#ifdef _TG_WINDOWS_
	struct _timeb after_win;

	_ftime(&after_win);
	back.time.tv_sec = (long)after_win.time;
	back.time.tv_usec = (long)after_win.millitm * 1000;
	back.time.tv_nsec = 0;
#else
	struct timeval after;

	gettimeofday(&after,NULL);
	back.time.tv_sec = after.tv_sec;
	back.time.tv_usec = after.tv_usec;
	back.time.tv_nsec = 0;
#endif
	back.quality = Tango::ATTR_VALID;
	back.name = CORBA::string_dup("Status");
	back.r_dim.dim_x = 1;
	back.r_dim.dim_y = 0;
	back.w_dim.dim_x = 0;
	back.w_dim.dim_y = 0;
}

			   
} // End of Tango namespace
