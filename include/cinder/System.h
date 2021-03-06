/*
 Copyright (c) 2010, The Barbarian Group
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "cinder/Cinder.h"

#include <vector>
#include <string>
#include <iostream>

namespace cinder { 

/** \brief Queries system software and hardware capabilities of the computer.
	Values are cached so there is low overhead in calling these functions more than once. **/
class System {
 public:
	//! Returns whether the system supports the SSE2 instruction set.
	static bool			hasSse2();
	//! Returns whether the system supports the SSE3 instruction set.	
	static bool			hasSse3();
	//! Returns whether the system supports the SSE4.1 instruction set.	
	static bool			hasSse4_1();
	//! Returns whether the system supports the SSE4.2 instruction set.		
	static bool			hasSse4_2();
	//! Returns whether the system supports the x86-64 instruction set.		
	static bool			hasX86_64();
	//! Returns the number of physical processors in the system. A single processor dual core machine returns 1.
	static int			getNumCpus();
	//! Returns the number of cores (or logical processors) in the system. A single processor dual core machine returns 2.	
	static int			getNumCores();
	//! Returns the major version of the operating system.
	//! For version \c 10.5.8, this is \c 10. For Windows Vista this is 6. Refer to the MSDN documentation for the \c OSVERSIONINFOEX struct for Windows meanings
	static int			getOsMajorVersion();
	//! Returns the minor version of the operating system.
	//! For version \c 10.5.8, this is \c 5. For Windows Vista this is 0. Refer to the MSDN documentation for the \c OSVERSIONINFOEX struct for Windows meanings
	static int			getOsMinorVersion();	
	//! Returns the bugfix version of the operating system.
	//! For version \c 10.5.8, this is \c 8. For Windows this corresponds to the major version of the service pack. So \c Service Pack 2 returns \c 2
	static int			getOsBugFixVersion();

	//! Returns whether the system supports MultiTouch. Also returns true under Windows 7 in the presence of single touch support. Always returns true on Mac OS X Snow Leopard.
	static bool			hasMultiTouch();
	//! Returns the maximum number of simultaneous touches supported by the system's MultiTouch implementation. Only truly accurate on Windows 7.
	static int32_t		getMaxMultiTouchPoints();
	
	//! Represents a single Network Adapter of the system
	class NetworkAdapter {
	  public:
		const std::string&	getName() const { return mName; }
		const std::string&	getIpAddress() const { return mIpAddress; }

		NetworkAdapter( const std::string &name, const std::string &ip )
			: mName( name ), mIpAddress( ip ) {}

	  private:
		std::string		mName, mIpAddress;
	};
	//! Returns a list of the network adapters associated with the machine. Not cached.
	static std::vector<NetworkAdapter>		getNetworkAdapters();
	//! Returns a best guess at the machine's "IP address". Not cached. Computers often have multiple IP addresses, but this will attempt to select the "best". \sa getNetworkAdapaters().
	static std::string						getIpAddress();
	
 private:
	 enum {	HAS_SSE2, HAS_SSE3, HAS_SSE4_1, HAS_SSE4_2, HAS_X86_64, PHYSICAL_CPUS, LOGICAL_CPUS, OS_MAJOR, OS_MINOR, OS_BUGFIX, MULTI_TOUCH, MAX_MULTI_TOUCH_POINTS, TOTAL_CACHE_TYPES };

	System();
	static std::shared_ptr<System>		instance();
	static std::shared_ptr<System>		sInstance;

	bool				mCachedValues[TOTAL_CACHE_TYPES];
	bool				mHasSSE2, mHasSSE3, mHasSSE4_1, mHasSSE4_2, mHasX86_64;
	int					mPhysicalCPUs, mLogicalCPUs;
	int32_t				mOSMajorVersion, mOSMinorVersion, mOSBugFixVersion;
	bool				mHasMultiTouch;
	uint32_t			mMaxMultiTouchPoints;
#if defined( CINDER_MSW )
	uint32_t			mCPUID_EBX, mCPUID_ECX, mCPUID_EDX;
#endif

#if defined( CINDER_LINUX )
	static void linuxOsVersions();
#endif
};

inline std::ostream& operator<<( std::ostream &outp, const System::NetworkAdapter &adapter )
{
	outp << adapter.getName() << std::string(": IP: ") << adapter.getIpAddress();
	return outp;
}

class SystemExc : public std::exception {
};

class SystemExcFailedQuery : public SystemExc {
};

} // namespace cinder
