static const char *RcsId = "$Id$\n$Name$";

//+============================================================================
//
// file :               BlackBox.cpp
//
// description :        C++ source code for the BlackBoxElt and BlackBox
//			classes. These classes are used to implement the 
//			tango device server black box. There is one
//			black box for each Tango device. This black box
//			keeps info. on all the activities on a device.
//			A client is able to retrieve these data via a Device
//			attribute
//
// project :            TANGO
//
// author(s) :          A.Gotz + E.Taurel
//
// $Revision$
//
// $Log$
// Revision 3.12  2008/03/14 11:53:52  taurel
// - Add a check if the calling thread has been created by omni_thread
// (For thread created by Python)
//
// Revision 3.11  2007/04/20 14:40:24  taurel
// - Ported to Windows 64 bits x64 architecture
//
// Revision 3.10  2006/05/18 08:52:37  taurel
// - Miscellaneous changes due to Python device server ported to Windows
// - Fix some bugs discovered by Windows VC8 using the test suite
// - Update Windows resource file include path
// - Fix some Windows VC8 warnings
//
// Revision 3.9  2005/01/13 08:27:32  taurel
// - Merge trunk with Release_5_0 from brach Release_5_branch
//
// Revision 3.8.2.2  2004/10/22 11:25:00  taurel
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
// Revision 3.8.2.1  2004/08/19 07:44:03  taurel
// - Replace server low level database access call by Database class method call
// - Split device monitor in 3 : 1 to protect harware access, 1 to protect cache access and one mutex for device black box
//
// Revision 3.8  2004/07/07 08:39:56  taurel
//
// - Fisrt commit after merge between Trunk and release 4 branch
// - Add EventData copy ctor, asiignement operator and dtor
// - Add Database and DeviceProxy::get_alias() method
// - Add AttributeProxy ctor from "device_alias/attribute_name"
// - Exception thrown when subscribing two times for exactly yhe same event
//
// Revision 3.7  2003/08/21 07:23:46  taurel
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
// Revision 3.6  2003/07/03 07:40:51  taurel
// - Change in Tango IDL file : Implement a new way to tranfer data for read_attribute and write_attribute CORBA operation
// - Handle this new IDL release in DeviceProxy class
// - New exception methods in DeviceAttribute class
// - New way to get data out of DeviceAttribute object
// - Fix bugs in DeviceProxy copy constructor and assignement operator
// - Change some method names in DeviceDataHistory and DeviceAttributeHistory classes
// - Change the implementation of the DeviceProxy::write_attribute() method to avoid DeviceAttribute copying
// - Clean-up how a server is killed via a CTRL-C or a dserver device kill command
// - Add a server_cleanup() method in the Util class
// - Win32 : Update debug menu in the server graphical window to support logging feature
// - Win32 : Display library CVS tag in the "Help->About" sub-window
//
// Revision 3.5.2.2  2004/03/09 16:36:36  taurel
// - Added HP aCC port (thanks to Claudio from Elettra)
// - Some last small bugs fixes
//
// Revision 3.5.2.1  2003/09/30 11:49:56  taurel
// Add some changes foreseen for release 4.1 and already implemented on
// the trunck into this release 4.0 branch
//
// Revision 3.5  2003/06/23 09:04:17  taurel
// Fix no-end memory increase bug in the insert_attr() methods. Clear the
// vector before inserting new attribute names !!
//
// Revision 3.4  2003/05/28 14:55:07  taurel
// Add the include (conditionally) of the include files generated by autoconf
//
// Revision 3.3  2003/05/16 08:46:15  taurel
// Many changes for release 3.0.1. The most important ones are :
// - Timeout are backs
// - Multiple db servers (change in TANGO_HOST syntax)
// - Added methods to print DeviceData, DeviceDataHistory, DeviceAttribute and DeviceAttributeHistory instances
// - Attributes name stored in blackbox
// - Remove check if a class is created without any device
// - It's now possible to create a DeviceProxy from its alias name
// - Command, attribute names are case insensitive
// - Change parameters of some DeviceProxy logging methods
// - Change parameters of DeviceProxy asynchronous replies calls
// - New serialization model in device server (no serialization model)
// - Win32 (2000) device server service does not exit at loggoff anymore
// - Miscellaneous bug fixes
//
// Revision 3.2  2003/04/24 10:18:08  nleclercq
// Minor modifications to make some compilers happy
//
// Revision 3.1  2003/04/08 15:06:16  taurel
// - Names of read/written attributes are now stored in blackbox
// - Admin device RestartServer command is now done in its own thread with
//   changing CORBA POA manager state
//
// Revision 3.0  2003/03/25 16:41:58  taurel
// Many changes for Tango release 3.0 including
// - Added full logging features
// - Added asynchronous calls
// - Host name of clients now stored in black-box
// - Three serialization model in DS
// - Fix miscellaneous bugs
// - Ported to gcc 3.2
// - Added ApiUtil::cleanup() and destructor methods
// - Some internal cleanups
// - Change the way how TangoMonitor class is implemented. It's a recursive
//   mutex
//
// Revision 2.9  2003/01/09 12:03:15  taurel
// - Ported to gcc 3.2
// - Added ApiUtil::cleanup() and ApiUtil::~ApiUtil() methods
// - Replace some ORB * by ORB_ptr
// - Use CORBA::ORB::is_nil() instead of comparing to NULL
//
// Revision 2.8  2002/12/16 12:06:21  taurel
// No change in code at all but only forgot th emost important line in
// list of updates in the previous release :
// - Change underlying ORB from ORBacus to omniORB
//
// Revision 2.7  2002/12/16 10:15:35  taurel
// - New method get_device_list() in Util class
// - Util::get_class_list takes DServer device into account
// - Util::get_device_by_name() takes DServer device into account
// - Util::get_device_list_by_class() takes DServer device into account
// - New parameter to the attribute::set_value() method to enable CORBA to free
// memory allocated for the attribute
//
// Revision 2.6  2002/10/17 07:43:05  taurel
// Fix bug in history stored by the polling thread :
// - We need one copy of the attribute data to build an history!!! It is true
// also for command which return data created by the DeviceImpl::create_xxx
// methods. Chnage in pollring.cpp/pollring.h/dserverpoll.cpp/pollobj.cpp
// and pollobj.h
//
// Revision 2.5  2002/10/15 11:27:18  taurel
// Fix bugs in device.cpp file :
// - Protect the state and status CORBA attribute with the device monitor
// Add the "TgLibVers" string as a #define in tango_config.h
//
// Revision 2.4  2002/08/12 15:06:53  taurel
// Several big fixes and changes
//   - Remove HP-UX specific code
//   - Fix bug in polling alogorithm which cause the thread to enter an infinite
//     loop (pollthread.cpp)
//   - For bug for Win32 device when trying to set attribute config
//     (attribute.cpp)
//
// Revision 2.3  2002/07/02 15:22:23  taurel
// Miscellaneous small changes/bug fixes for Tango CPP release 2.1.0
//     - classes reference documentation now generated using doxygen instead of doc++
//     - A little file added to the library which summarizes version number.
//       The RCS/CVS "ident" command will now tells you that release library x.y.z is composed
//       by C++ client classes set release a.b and C++ server classes set release c.d
//     - Fix incorrect field setting for DevFailed exception re-thrown from a CORBA exception
//     - It's now not possible to poll the Init command
//     - It's now possible to define a default class doc. per control system
//       instance (using property)
//     - The test done to check if attribute value has been set before it is
//       returned to caller is done only if the attribute quality is set to VALID
//     - The JTCInitialize object is now stored in the Util
//     - Windows specific : The tango.h file now also include winsock.h
//
// Revision 2.2  2002/04/30 10:50:40  taurel
// Don't check alarm on attribute if attribute quality factor is INVALID
//
// Revision 2.1  2002/04/29 12:24:02  taurel
// Fix bug in attribute::set_value method and on the check against min and max value when writing attributes
//
// Revision 2.0  2002/04/09 14:45:08  taurel
// See Tango WEB pages for list of changes
//
// Revision 1.6  2001/10/08 09:03:10  taurel
// See tango WEB pages for list of changes
//
// Revision 1.5  2001/07/04 12:27:09  taurel
// New methods re_throw_exception(). Read_attributes supports AllAttr mnemonic A new add_attribute()method in DeviceImpl class New way to define attribute properties New pattern to prevent full re-compile For multi-classes DS, it is now possible to use the Util::get_device_by_name() method in device constructor Adding << operator ovebloading Fix devie CORBA ref. number when device constructor sends an excep.
//
// Revision 1.4  2001/05/04 09:28:12  taurel
// Fix bugs in DServer::restart() method and in Util::get_device_by_name() method
//
// Revision 1.3  2001/03/30 08:03:44  taurel
// Fix bugs in attributes. For linux, add signal_handler in its own thread, change the way to kill server. For all system, change DevRestart philosophy.
//
// Revision 1.2  2001/03/09 08:20:14  taurel
// Fix bug in the MultiClassAttribute::init_class_attribute() method. Also remove the DbErr_DeviceNotDefined define.
//
// Revision 1.1.1.1  2001/02/27 08:46:20  taurel
// Imported sources
//
// Revision 1.3  2000/04/13 10:40:39  taurel
// Added attribute support
//
// Revision 1.2  2000/02/04 11:00:13  taurel
// Just update revision number
//
// Revision 1.1.1.1  2000/02/04 10:58:27  taurel
// Imported sources
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
#include <blackbox.h>

#include <stdio.h>

#ifdef _TG_WINDOWS_
#include <sys/types.h>
#include <sys/timeb.h>
#else
#include <sys/time.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#endif /* _TG_WINDOWS_ */

#include <omniORB4/omniInterceptors.h>
#include <omniORB4/internal/giopStrand.h>
#include <omniORB4/internal/giopStream.h>
#include <omniORB4/internal/GIOP_S.h>


namespace Tango
{

//
// The per thread data storage key (The client IP is stored in thread specific storage)
// defined in utils.cpp
//

extern omni_thread::key_t key;

//
// The function called by the interceptor
//

CORBA::Boolean get_client_addr(omni::omniInterceptors::serverReceiveRequest_T::info_T &info)
{
	omni_thread::self()->set_value(key,new client_addr(((omni::giopStrand &)info.giop_s).connection->peeraddress()));
	return true;
}

//+-------------------------------------------------------------------------
//
// method : 		BlackBoxElt::BlackBoxElt 
// 
// description : 	Constructor for the BlackBoxElt class.
//			This constructor simply set the internal value to their
//			default
//
//--------------------------------------------------------------------------

BlackBoxElt::BlackBoxElt()
{
	req_type = Req_Unknown;
	attr_type = Attr_Unknown;
	op_type = Op_Unknown;
	when.tv_sec = when.tv_usec = 0;
	host_ip_str[0] = '\0';
	attr_names.reserve(DEFAULT_ATTR_NB);
}

BlackBoxElt::~BlackBoxElt()
{
}

//+-------------------------------------------------------------------------
//
// method : 		BlackBox::BlackBox 
// 
// description : 	Two constructors for the BlackBox class. The first one
//			does not take any argument and construct a black box
//			with the default depth.
//			The second one create a black box with a depth defined 
//			by the argument.
//
// argument : in : 	- max_size : The black box depth
//
//--------------------------------------------------------------------------

BlackBox::BlackBox():box(DefaultBlackBoxDepth)
{
	insert_elt = 0;
	nb_elt = 0;
	max_elt = DefaultBlackBoxDepth;	
}

BlackBox::BlackBox(long max_size):box(max_size)
{
	insert_elt = 0;
	nb_elt = 0;
	max_elt = max_size;		
}

//+-------------------------------------------------------------------------
//
// method : 		BlackBox::insert_corba_attr
// 
// description : 	This method insert a new element in the black box when
//			this element is a attribute
//
// argument : in : 	- attr : The attribute type
//
//--------------------------------------------------------------------------


void BlackBox::insert_corba_attr(BlackBoxElt_AttrType attr)
{

//
// Take mutex
//

	sync.lock();
			
//
// Insert elt in the box
//

	box[insert_elt].req_type = Req_Attribute;
	box[insert_elt].attr_type = attr;
	box[insert_elt].op_type = Op_Unknown;

#ifdef _TG_WINDOWS_
//
// Note that the exact conversion between milli-sec and u-sec will be done
// only when data is send back to user. This save some times in unnecessary
// computation
//
	struct _timeb t;
	_ftime(&t);
	
	box[insert_elt].when.tv_usec = (long)t.millitm;
	box[insert_elt].when.tv_sec = (unsigned long)t.time;
#else	
	struct timezone tz;
	gettimeofday(&box[insert_elt].when,&tz);
#endif

//
// get client address
//

	get_client_host();
			
//
// manage insert and read indexes
//

	inc_indexes();

//
// Release mutex
//

	sync.unlock();
}

//+-------------------------------------------------------------------------
//
// method : 		BlackBox::insert_cmd
// 
// description : 	This method insert a new element in the black box when
//			this element is a call to the operation command_inout
//
// argument : in : 	- c_name : The command name
//
//--------------------------------------------------------------------------


void BlackBox::insert_cmd(const char *cmd,long vers,DevSource sour)
{

//
// Take mutex
//

	sync.lock();
		
//
// Insert elt in the box
//

	box[insert_elt].req_type = Req_Operation;
	box[insert_elt].attr_type = Attr_Unknown;
	if (vers == 1)
		box[insert_elt].op_type = Op_Command_inout;
	else
		box[insert_elt].op_type = Op_Command_inout_2;
	box[insert_elt].cmd_name = cmd;
	box[insert_elt].source = sour;
#ifdef _TG_WINDOWS_
//
// Note that the exact conversion between milli-sec and u-sec will be done
// only when data is send back to user. This save some times in unnecessary
// computation
//
	struct _timeb t;
	_ftime(&t);
	
	box[insert_elt].when.tv_usec = (long)t.millitm;
	box[insert_elt].when.tv_sec = (unsigned long)t.time;
#else	
	struct timezone tz;
	gettimeofday(&box[insert_elt].when,&tz);
#endif

//
// get client address
//

	get_client_host();
		
//
// manage insert and read indexes
//

	inc_indexes();
	
//
// Release mutex
//

	sync.unlock();
}

//+-------------------------------------------------------------------------
//
// method : 		BlackBox::insert_op
// 
// description : 	This method insert a new element in the black box when
//			this element is a call to an operation which is not
//			the command_inout operation
//
// argument : in : 	- cmd : The operation type
//
//--------------------------------------------------------------------------


void BlackBox::insert_op(BlackBoxElt_OpType op)
{

//
// Take mutex
//

	sync.lock();
	
//
// Insert elt in the box
//

	box[insert_elt].req_type = Req_Operation;
	box[insert_elt].attr_type = Attr_Unknown;
	box[insert_elt].op_type = op;
#ifdef _TG_WINDOWS_
//
// Note that the exact conversion between milli-sec and u-sec will be done
// only when data is send back to user. This save some times in unnecessary
// computation
//
	struct _timeb t;
	_ftime(&t);
	
	box[insert_elt].when.tv_usec = (long)t.millitm;
	box[insert_elt].when.tv_sec = (unsigned long)t.time;
#else	
	struct timezone tz;
	gettimeofday(&box[insert_elt].when,&tz);
#endif

//
// get client address
//

	get_client_host();
		
//
// manage insert and read indexes
//

	inc_indexes();
	
//
// Release mutex 
//

	sync.unlock();
}

//+-------------------------------------------------------------------------
//
// method : 		BlackBox::insert_attr
// 
// description : 	This method insert a new element in the black box when
//			this element is a call to the CORBA operation 
//			read_attributes
//
// argument : in : 	- names : The attribute(s) name
//
//--------------------------------------------------------------------------


void BlackBox::insert_attr(const Tango::DevVarStringArray &names,long vers,DevSource sour)
{

//
// Take mutex
//

	sync.lock();
	
//
// Insert elt in the box
//

	box[insert_elt].req_type = Req_Operation;
	box[insert_elt].attr_type = Attr_Unknown;
	switch (vers)
	{
	case 1 :
		box[insert_elt].op_type = Op_Read_Attr;
		break;
		
	case 2 :
		box[insert_elt].op_type = Op_Read_Attr_2;
		break;
		
	case 3 :
		box[insert_elt].op_type = Op_Read_Attr_3;
		break;
	}
	box[insert_elt].source = sour;
	

	box[insert_elt].attr_names.clear();
	for (unsigned long i = 0;i < names.length();i++)
	{
		string tmp_str(names[i]);
		box[insert_elt].attr_names.push_back(tmp_str);
	}
	
#ifdef _TG_WINDOWS_
//
// Note that the exact conversion between milli-sec and u-sec will be done
// only when data is send back to user. This save some times in unnecessary
// computation
//
	struct _timeb t;
	_ftime(&t);
	
	box[insert_elt].when.tv_usec = (long)t.millitm;
	box[insert_elt].when.tv_sec = (unsigned long)t.time;
#else	
	struct timezone tz;
	gettimeofday(&box[insert_elt].when,&tz);
#endif

//
// get client address
//

	get_client_host();
		
//
// manage insert and read indexes
//

	inc_indexes();
	
//
// Release mutex
//

	sync.unlock();
}

void BlackBox::insert_attr(const Tango::AttributeValueList &att_list, long vers)
{

//
// Take mutex
//

	sync.lock();
	
//
// Insert elt in the box
//

	box[insert_elt].req_type = Req_Operation;
	box[insert_elt].attr_type = Attr_Unknown;
	if (vers == 1)
		box[insert_elt].op_type = Op_Write_Attr;
	else
		box[insert_elt].op_type = Op_Write_Attr_3;

	box[insert_elt].attr_names.clear();
	for (unsigned long i = 0;i < att_list.length();i++)
	{
		string tmp_str(att_list[i].name);
		box[insert_elt].attr_names.push_back(tmp_str);
	}
	
#ifdef _TG_WINDOWS_
//
// Note that the exact conversion between milli-sec and u-sec will be done
// only when data is send back to user. This save some times in unnecessary
// computation
//
	struct _timeb t;
	_ftime(&t);
	
	box[insert_elt].when.tv_usec = (long)t.millitm;
	box[insert_elt].when.tv_sec = (unsigned long)t.time;
#else	
	struct timezone tz;
	gettimeofday(&box[insert_elt].when,&tz);
#endif

//
// get client address
//

	get_client_host();
		
//
// manage insert and read indexes
//

	inc_indexes();
	
//
// Release mutex
//

	sync.unlock();
}


//+-------------------------------------------------------------------------
//
// method : 		BlackBox::inc_indexes
// 
// description : 	This private method increment the indexes used to acces
//			the box itself. This is necessary because the box must
//			be managed as a circular buffer
//
//--------------------------------------------------------------------------


void BlackBox::inc_indexes()
{
	insert_elt++;
	if (insert_elt == max_elt)
		insert_elt = 0;
		
	if (nb_elt != max_elt)
		nb_elt++;
}

//+-------------------------------------------------------------------------
//
// method : 		get_client_host
// 
// description : 	This private method retrieves the client host IP
//			address (the number). IT USES OMNIORB SPECIFIC
//			INTERCEPTOR
//
//--------------------------------------------------------------------------

void BlackBox::get_client_host()
{
	omni_thread *th_id = omni_thread::self();
	if (th_id == NULL)
		th_id = omni_thread::create_dummy();
	
	omni_thread::value_t *ip = th_id->get_value(key);
	if (ip == NULL)
		strcpy(box[insert_elt].host_ip_str,"polling");
	else
		strcpy(box[insert_elt].host_ip_str,
	       	       ((client_addr *)(ip))->client_ip);
}
	
//+-------------------------------------------------------------------------
//
// method : 		BlackBox::build_info_as_str
// 
// description : 	Translate all the info stored in a black box element
//			into a readable string.
//
// argument : in : 	- index : The black box element index
//
//--------------------------------------------------------------------------

void BlackBox::build_info_as_str(long index)
{
	char date_str[25];
//
// Convert time to a string
//

	date_ux_to_str(box[index].when,date_str);
	elt_str = date_str; 
	
//
// Add request type and command name in case of
//

	elt_str = elt_str + " : ";
	unsigned long nb_in_vect;
	if (box[index].req_type == Req_Operation)
	{
		elt_str = elt_str + "Operation ";
		unsigned long i;
		switch (box[index].op_type)
		{
		case Op_Command_inout :
			elt_str = elt_str + "command_inout (cmd = " + box[index].cmd_name + ") from ";
			switch (box[index].source)
			{
			case DEV : 
				elt_str = elt_str + "device ";
				break;
				
			case CACHE :
				elt_str = elt_str + "cache ";
				break;
				
			case CACHE_DEV :
				elt_str = elt_str + "cache_device ";
				break;
				
			default :
				elt_str = elt_str + "unknown source (!) ";
				break;
			}
			break;
			
		case Op_Ping :
			elt_str = elt_str + "ping ";
			break;
			
		case Op_Info :
			elt_str = elt_str + "info ";
			break;

		case Op_BlackBox :
			elt_str = elt_str + "blackbox ";
			break;

		case Op_Command_list :
			elt_str = elt_str + "command_list_query ";
			break;
		
		case Op_Command :
			elt_str = elt_str + "command_query ";
			break;
										
		case Op_Get_Attr_Config :
			elt_str = elt_str + "get_attribute_config ";
			break;
			
		case Op_Set_Attr_Config :
			elt_str = elt_str + "set_attribute_config ";
			break;
			
		case Op_Read_Attr :
			elt_str = elt_str + "read_attributes (";
			nb_in_vect = box[index].attr_names.size();
			for (i = 0;i < nb_in_vect;i++)
			{
				elt_str = elt_str + box[index].attr_names[i];
				if (i != nb_in_vect - 1)
					elt_str = elt_str + ", ";
			}
			elt_str = elt_str + ") from ";
			switch (box[index].source)
			{
			case DEV : 
				elt_str = elt_str + "device ";
				break;
				
			case CACHE :
				elt_str = elt_str + "cache ";
				break;
				
			case CACHE_DEV :
				elt_str = elt_str + "cache_device ";
				break;
				
			default :
				elt_str = elt_str + "unknown source (!) ";
				break;
			}
			break;
			
		case Op_Write_Attr :
			elt_str = elt_str + "write_attributes (";
			nb_in_vect = box[index].attr_names.size();
			for (i = 0;i < nb_in_vect;i++)
			{
				elt_str = elt_str + box[index].attr_names[i];
				if (i != nb_in_vect - 1)
					elt_str = elt_str + ", ";
			}
			elt_str = elt_str + ") ";
			break;
			
		case Op_Write_Attr_3 :
			elt_str = elt_str + "write_attributes_3 (";
			nb_in_vect = box[index].attr_names.size();
			for (i = 0;i < nb_in_vect;i++)
			{
				elt_str = elt_str + box[index].attr_names[i];
				if (i != nb_in_vect - 1)
					elt_str = elt_str + ", ";
			}
			elt_str = elt_str + ") ";
			break;
			
		case Op_Command_inout_2 :
			elt_str = elt_str + "command_inout_2 (cmd = " + box[index].cmd_name + ") from ";
			switch (box[index].source)
			{
			case DEV : 
				elt_str = elt_str + "device ";
				break;
				
			case CACHE :
				elt_str = elt_str + "cache ";
				break;
				
			case CACHE_DEV :
				elt_str = elt_str + "cache_device ";
				break;
				
			default :
				elt_str = elt_str + "unknown source (!) ";
				break;
			}
			break;
			
		case Op_Command_list_2 :
			elt_str = elt_str + "command_list_query_2 ";
			break;
			
		case Op_Command_2 :
			elt_str = elt_str + "command_query_2 ";
			break;
			
		case Op_Get_Attr_Config_2 :
			elt_str = elt_str + "get_attribute_config_2 ";
			break;
			
		case Op_Read_Attr_2 :
			elt_str = elt_str + "read_attributes_2 (";
			nb_in_vect = box[index].attr_names.size();
			for (i = 0;i < nb_in_vect;i++)
			{
				elt_str = elt_str + box[index].attr_names[i];
				if (i != nb_in_vect - 1)
					elt_str = elt_str + ", ";
			}
			elt_str = elt_str + ") from ";
			switch (box[index].source)
			{
			case DEV : 
				elt_str = elt_str + "device ";
				break;
				
			case CACHE :
				elt_str = elt_str + "cache ";
				break;
				
			case CACHE_DEV :
				elt_str = elt_str + "cache_device ";
				break;
				
			default :
				elt_str = elt_str + "unknown source (!) ";
				break;
			}			
			break;
			
		case Op_Read_Attr_3 :
			elt_str = elt_str + "read_attributes_3 (";
			nb_in_vect = box[index].attr_names.size();
			for (i = 0;i < nb_in_vect;i++)
			{
				elt_str = elt_str + box[index].attr_names[i];
				if (i != nb_in_vect - 1)
					elt_str = elt_str + ", ";
			}
			elt_str = elt_str + ") from ";
			switch (box[index].source)
			{
			case DEV : 
				elt_str = elt_str + "device ";
				break;
				
			case CACHE :
				elt_str = elt_str + "cache ";
				break;
				
			case CACHE_DEV :
				elt_str = elt_str + "cache_device ";
				break;
				
			default :
				elt_str = elt_str + "unknown source (!) ";
				break;
			}
			break;

		case Op_Command_inout_history_2 :
			elt_str = elt_str + "command_inout_history_2 ";
			break;
			
		case Op_Read_Attr_history_2 :
			elt_str = elt_str + "read_attribute_history_2 ";
			break;
			
		case Op_Read_Attr_history_3 :
			elt_str = elt_str + "read_attribute_history_3 ";
			break;
			
		case Op_Info_3 :
			elt_str = elt_str + "info_3 ";
			break;
			
		case Op_Get_Attr_Config_3 :
			elt_str = elt_str + "get_attribute_config_3 ";
			break;
			
		case Op_Set_Attr_Config_3 :
			elt_str = elt_str + "set_attribute_config_3 ";
			break;
			
		case Op_Unknown :
			elt_str = elt_str + "unknown operation !!!!!";
			return;
		}
	}	
	else if (box[index].req_type == Req_Attribute)
	{
		elt_str = elt_str + "Attribute ";
		switch (box[index].attr_type)
		{
		case Attr_Name :
			elt_str = elt_str + "name ";
			break;
			
		case Attr_Description :
			elt_str = elt_str + "description ";
			break;

		case Attr_Status :
			elt_str = elt_str + "status ";
			break;
			
		case Attr_State :
			elt_str = elt_str + "state ";
			break;
			
		case Attr_AdmName :
			elt_str = elt_str + "adm_name ";
			break;
						
		case Attr_Unknown :
			elt_str = elt_str + "unknown attribute !!!!!";
			return;
		}
	}
	else
	{
		elt_str = elt_str + "Unknown CORBA request type !!!!!";
		return;
	}

//
// Add client host name.
// Extract only IP numbers from the omni address format (giop:tcp:xx.yy.zz.kk:port)
// Return in case of badly formed address
//

	if ((box[index].host_ip_str[0] != '\0') && (box[index].host_ip_str[0] != 'p'))
	{
		string omni_addr = box[index].host_ip_str;
		string::size_type pos;
		if ((pos = omni_addr.find(':')) == string::npos)
			return;
		pos++;
		if ((pos = omni_addr.find(':',pos)) == string::npos)
			return;
		pos++;
		string ip_str = omni_addr.substr(pos);
		if ((pos = ip_str.find(':')) == string::npos)
			return;
		string full_ip_str = ip_str.substr(0,pos);
	
		if ((pos = full_ip_str.find('.')) == string::npos)
			return;
		string ip1_str = full_ip_str.substr(0,pos);
		string::size_type old_pos;
		pos++;
		old_pos = pos;
		if ((pos = full_ip_str.find('.',pos)) == string::npos)
			return;
		string ip2_str = full_ip_str.substr(old_pos,pos - old_pos);
		pos++;
		old_pos = pos;
		if ((pos = full_ip_str.find('.',pos)) == string::npos)
			return;
		string ip3_str = full_ip_str.substr(old_pos,pos - old_pos);
		pos++;
		string ip4_str = full_ip_str.substr(pos);
	
//
// Convert ip address field to numbers
//

		long ip1,ip2,ip3,ip4;
		ip1 = atol(ip1_str.c_str());
		ip2 = atol(ip2_str.c_str());
		ip3 = atol(ip3_str.c_str());
		ip4 = atol(ip4_str.c_str());

//
// Finally, get host name
//

        	struct hostent *ho;

#ifdef _TG_WINDOWS_
		struct in_addr ad;
		ad.s_addr = (ip1 << 24) + (ip2 << 16) + (ip3 << 8) + ip4;
		unsigned long ip_net = htonl(ad.s_addr);
		ho = gethostbyaddr((char *)(&ip_net),sizeof(ip_net),AF_INET);
#else
		unsigned long ip = (ip1 << 24) + (ip2 << 16) + (ip3 << 8) + ip4;
#ifdef __linux
		unsigned long ip_net = htonl(ip);
		ho = gethostbyaddr((char *)(&ip_net),sizeof(ip),AF_INET);
#else
		ho = gethostbyaddr((char *)(&ip),sizeof(ip),AF_INET);
#endif /* __linux */
#endif /* _TG_WINDOWS_ */
		
		if (ho != NULL)
		{
			elt_str = elt_str + "requested from ";
			elt_str = elt_str + ho->h_name;
		}
		else
		{
			elt_str = elt_str + "requested from ";
			elt_str = elt_str + full_ip_str;
		}
	}
	else if (box[index].host_ip_str[0] == 'p')
	{
		elt_str = elt_str + "requested from polling";
	}
	
	return;
}

//+-------------------------------------------------------------------------
//
// method : 		BlackBox::read
// 
// description : 	Read black box element as strings. The newest element
//			is return in the first position
//
// argument : in : 	- index : The number of element to read
//
//--------------------------------------------------------------------------

Tango::DevVarStringArray *BlackBox::read(long wanted_elt)
{


//
// Take mutex
//

	sync.lock();
	
//
// Throw exeception if the wanted element is stupid and if there is no element
// stored in the black box
//

	if (wanted_elt <= 0)
	{
		sync.unlock();
			
		Except::throw_exception((const char *)"API_BlackBoxArgument",
				      (const char *)"Argument to read black box out of range",
				      (const char *)"BlackBox::read");
	}
	if (nb_elt == 0)
	{
		sync.unlock();

		Except::throw_exception((const char *)"API_BlackBoxEmpty",
				      (const char *)"Nothing stored yet in black-box",
				      (const char *)"BlackBox::read");
	}
			
//
// Limit wanted element to a reasonable value
//

	if (wanted_elt > max_elt)
		wanted_elt = max_elt;
		
	if (wanted_elt > nb_elt)
		wanted_elt = nb_elt;
		
//
// Read black box elements
//

	Tango::DevVarStringArray *ret;
	try
	{
	
		ret = new Tango::DevVarStringArray(wanted_elt);
                ret->length(wanted_elt);

		long read_index;
		if (insert_elt == 0)
			read_index = max_elt - 1;
		else
			read_index = insert_elt - 1;
		for (long i = 0;i < wanted_elt;i++)
		{
			build_info_as_str(read_index);
			(*ret)[i] = elt_str.c_str();
		
			read_index--;
			if (read_index < 0)
				read_index = max_elt - 1;
		}
	}
	catch (bad_alloc)
	{
		sync.unlock();
			
		Except::throw_exception((const char *)"API_MemoryAllocation",
				      (const char *)"Can't allocate memory in server",
				      (const char *)"BlackBox::read");
	}
	
//
// Release mutex
//

	sync.unlock();
				
	return(ret);
}

//+-------------------------------------------------------------------------
//
// method : 		BlackBox::date_ux_to_str
// 
// description : 	Convert a UNIX date (number of seconds since EPOCH)
//			to a string of the following format :
//			dd/mm/yyyy hh24:mi:ss:xx
//
// argument : in : 	- ux_date : The UNIX date in a timeval structure
//			- str_date : Pointer to char array where the date will
//				     be stored (must be allocated)
//
//--------------------------------------------------------------------------

void BlackBox::date_ux_to_str(timeval &ux_date,char *str_date)
{
	long i;
	char month[5];
	char *ux_str;
	char unix_date[30];

/* Convert UNIX date to a string in UNIX format */

#ifdef WIN32_VC8
	time_t vc8_time;
	vc8_time = (time_t)ux_date.tv_sec;
	ux_str = ctime(&vc8_time);
#else
	ux_str = ctime((time_t *)&(ux_date.tv_sec));
#endif
	strcpy(unix_date,ux_str);

/* Copy day */

	for (i = 0;i < 2;i++)
		str_date[i] = unix_date[i + 8];
	str_date[2] = '/';
	str_date[3] = '\0';
	if (str_date[0] == ' ')
		str_date[0] = '0';

/* Copy month */

	for (i = 0;i < 3;i++)
		month[i] = unix_date[i + 4];
	month[3] = '\0';

	switch(month[0])
	{
		case 'J' : if (month[1] == 'u')
			   {
				if (month[2] == 'n')
					strcat(str_date,"06/");
				else
					strcat(str_date,"07/");
			   }
			   else
				strcat(str_date,"01/");
			   break;

		case 'F' : strcat(str_date,"02/");
			   break;

		case 'M' : if (month[2] == 'r')
				strcat(str_date,"03/");
			   else
				strcat(str_date,"05/");
			   break;

		case 'A' : if (month[1] == 'p')
				strcat(str_date,"04/");
			   else
				strcat(str_date,"08/");
			   break;

		case 'S' : strcat(str_date,"09/");
			   break;

		case 'O' : strcat(str_date,"10/");
		           break;

		case 'N' : strcat(str_date,"11/");
			   break;

		case 'D' : strcat(str_date,"12/");
			   break;
	}

	str_date[6] = '\0';

/* Copy year */

	strcat(str_date,&(unix_date[20]));
	str_date[10] = '\0';
	
/* Copy date remaining */

	strcat(str_date," ");
	for (i = 0;i < 8;i++)
		str_date[i + 11] = unix_date[i + 11];
	str_date[19] = '\0';
	
/* Add milliseconds */

#ifdef _TG_WINDOWS_
	sprintf(&(str_date[19]),":%.2d",(int)(ux_date.tv_usec/10));
#else
	sprintf(&(str_date[19]),":%.2d",(int)(ux_date.tv_usec/10000));
#endif

}

} // End of Tango namespace
