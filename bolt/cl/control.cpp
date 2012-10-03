#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

#include "bolt/cl/bolt.h"
#include "bolt/cl/control.h"

static const std::streamsize colWidth = 38;

void printExtention( const std::string& str )
{
    std::cout << std::setw( colWidth ) << "" << str << std::endl;
}

class printDeviceFunctor
{
    cl_uint m_numDev;

public:
    printDeviceFunctor( cl_uint initNumDevice ): m_numDev( initNumDevice )
    {}

    void operator()( const cl::Device& dev )
    {
        cl_int err = CL_SUCCESS;

        std::string strDeviceName = dev.getInfo< CL_DEVICE_NAME >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_NAME > failed" );

        std::string strDeviceProfile = dev.getInfo< CL_DEVICE_PROFILE >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_PROFILE > failed" );

        std::string strDeviceVersion = dev.getInfo< CL_DEVICE_VERSION >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_VERSION > failed" );

        std::string strDriverVersion = dev.getInfo< CL_DRIVER_VERSION >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DRIVER_VERSION > failed" );

        std::string strOpenCLVersion = dev.getInfo< CL_DEVICE_OPENCL_C_VERSION >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_OPENCL_C_VERSION > failed" );

        cl_device_type ulDeviceType = dev.getInfo< CL_DEVICE_TYPE >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_TYPE > failed" );

        cl_uint uiMaxClock = dev.getInfo< CL_DEVICE_MAX_CLOCK_FREQUENCY >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_MAX_CLOCK_FREQUENCY > failed" );

        cl_uint uiMaxUnits = dev.getInfo< CL_DEVICE_MAX_COMPUTE_UNITS >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_MAX_COMPUTE_UNITS > failed" );

        size_t stMaxWorkGroup = dev.getInfo< CL_DEVICE_MAX_WORK_GROUP_SIZE >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_MAX_WORK_GROUP_SIZE > failed" );

        cl_uint uiMaxDim = dev.getInfo< CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS > failed" );

        std::vector< size_t > uiMaxDimSizes = dev.getInfo< CL_DEVICE_MAX_WORK_ITEM_SIZES >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_MAX_WORK_ITEM_SIZES > failed" );

        std::string szDeviceExtensions = dev.getInfo< CL_DEVICE_EXTENSIONS >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_EXTENSIONS > failed" );

        cl_uint uiMemBaseAddrAlign = dev.getInfo< CL_DEVICE_MEM_BASE_ADDR_ALIGN >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_MEM_BASE_ADDR_ALIGN > failed" );

        cl_uint uiMinDataTypeAlignSize = dev.getInfo< CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE > failed" );

        cl_ulong ulGlobalMemSize = dev.getInfo< CL_DEVICE_GLOBAL_MEM_SIZE >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_GLOBAL_MEM_SIZE > failed" );

        cl_ulong ulMaxMemAllocSize = dev.getInfo< CL_DEVICE_MAX_MEM_ALLOC_SIZE >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_MAX_MEM_ALLOC_SIZE > failed" );

        cl_ulong ulLocalMemSize = dev.getInfo< CL_DEVICE_LOCAL_MEM_SIZE >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_LOCAL_MEM_SIZE > failed" );

        cl_device_local_mem_type ulLocalMemType = dev.getInfo< CL_DEVICE_LOCAL_MEM_TYPE >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_LOCAL_MEM_TYPE > failed" );

        cl_bool bHostUnifiedMem = dev.getInfo< CL_DEVICE_HOST_UNIFIED_MEMORY >( &err );
        bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_HOST_UNIFIED_MEMORY > failed" );

        //  Create a vector of extention strings, which we know are seperated by spaces
        std::istringstream splitExtentions( szDeviceExtensions );
        std::vector< std::string > extTokens;
        std::copy( std::istream_iterator< std::string >( splitExtentions ), std::istream_iterator< std::string >( ),
                   std::back_inserter< std::vector< std::string > >( extTokens ) );

        //  Print logic
        std::cout << "Device info [ " << m_numDev++ << " ]" << std::endl;
        std::cout << std::setw( colWidth ) << "CL_DEVICE_NAME : " << strDeviceName << std::endl;
        std::cout << std::setw( colWidth ) << "CL_DEVICE_TYPE : "
            << (CL_DEVICE_TYPE_DEFAULT     & ulDeviceType ? "default"     : "")
            << (CL_DEVICE_TYPE_CPU         & ulDeviceType ? "CPU"         : "")
            << (CL_DEVICE_TYPE_GPU         & ulDeviceType ? "GPU"         : "")
            << (CL_DEVICE_TYPE_ACCELERATOR & ulDeviceType ? "Accelerator" : "")
            << std::endl;

        std::cout << std::setw( colWidth ) << "CL_DEVICE_MAX_CLOCK_FREQUENCY : " << uiMaxClock << std::endl;
        std::cout << std::setw( colWidth ) << "CL_DEVICE_MAX_COMPUTE_UNITS : " << uiMaxUnits << std::endl;
        std::cout << std::setw( colWidth ) << "CL_DEVICE_MAX_WORK_GROUP_SIZE : " << stMaxWorkGroup << std::endl;
        std::cout << std::setw( colWidth ) << "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS : " << uiMaxDim << std::endl;
        for( size_t wis = 0; wis < uiMaxDimSizes.size( ); ++wis )
        {
            std::stringstream dimString;
            dimString << "Dimension[ " << wis << " ] : ";
            std::cout << std::setw( colWidth ) << dimString.str( ) << uiMaxDimSizes[ wis ] << std::endl;
        }
        std::cout << std::setw( colWidth ) << "CL_DEVICE_GLOBAL_MEM_SIZE : " << ulGlobalMemSize / 1024 / 1024 << " MB" << std::endl;
        std::cout << std::setw( colWidth ) << "CL_DEVICE_MAX_MEM_ALLOC_SIZE : " << ulMaxMemAllocSize / 1024 / 1024 << " MB" << std::endl;
        std::cout << std::setw( colWidth ) << "CL_DEVICE_MEM_BASE_ADDR_ALIGN : " << uiMemBaseAddrAlign << std::endl;
        std::cout << std::setw( colWidth ) << "CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE : " << uiMinDataTypeAlignSize << std::endl;
        std::cout << std::setw( colWidth ) << "CL_DEVICE_LOCAL_MEM_SIZE : " << ulLocalMemSize / 1024 << " KB" << std::endl;
        std::cout << std::setw( colWidth ) << "CL_DEVICE_LOCAL_MEM_TYPE : "
            << (CL_LOCAL     & ulLocalMemType ? "local"  : "")
            << (CL_GLOBAL    & ulLocalMemType ? "global" : "")
            << std::endl;
        std::cout << std::setw( colWidth ) << "CL_DEVICE_HOST_UNIFIED_MEMORY : "
            << (1 == bHostUnifiedMem ? "true"  : "")
            << (0 == bHostUnifiedMem ? "false" : "")
            << std::endl;

        std::cout << std::setw( colWidth ) << "CL_DEVICE_PROFILE : " << strDeviceProfile << std::endl;
        std::cout << std::setw( colWidth ) << "CL_DEVICE_OPENCL_C_VERSION : " << strOpenCLVersion << std::endl;
        std::cout << std::setw( colWidth ) << "CL_DEVICE_VERSION : " << strDeviceVersion << std::endl;
        std::cout << std::setw( colWidth ) << "CL_DRIVER_VERSION : " << strDriverVersion << std::endl;
        std::cout << std::setw( colWidth ) << "CL_DEVICE_EXTENSIONS : " << std::endl;
        std::for_each( extTokens.begin( ), extTokens.end( ), printExtention );
        std::cout << std::endl;
    }
};

class printPlatformFunctor
{
    cl_uint m_numPlat;
    cl_device_type m_deviceType;

public:
    printPlatformFunctor( cl_uint initNumPlat, cl_device_type deviceType ): 
        m_numPlat( initNumPlat ), m_deviceType( deviceType )
    {}

    void operator()( const cl::Platform& plat )
    {
        cl_int err = CL_SUCCESS;

        std::string szPlatformProfile = plat.getInfo< CL_PLATFORM_PROFILE >( &err );
        bolt::cl::V_OPENCL( err, "Platform::getInfo< CL_DEVICE_NAME > failed" );

        std::string szPlatformVersion = plat.getInfo< CL_PLATFORM_VERSION >( &err );
        bolt::cl::V_OPENCL( err, "Platform::getInfo< CL_DEVICE_NAME > failed" );

        std::string szPlatformName = plat.getInfo< CL_PLATFORM_NAME >( &err );
        bolt::cl::V_OPENCL( err, "Platform::getInfo< CL_DEVICE_NAME > failed" );

        std::string szPlatformVendor = plat.getInfo< CL_PLATFORM_VENDOR >( &err );
        bolt::cl::V_OPENCL( err, "Platform::getInfo< CL_DEVICE_NAME > failed" );

        std::string szPlatformExtensions = plat.getInfo< CL_PLATFORM_EXTENSIONS >( &err );
        bolt::cl::V_OPENCL( err, "Platform::getInfo< CL_DEVICE_NAME > failed" );

        //  Create a vector of extention strings, which we know are seperated by spaces
        std::istringstream splitExtentions( szPlatformExtensions );
        std::vector< std::string > extTokens;
        std::copy( std::istream_iterator< std::string >( splitExtentions ), std::istream_iterator< std::string >( ),
                   std::back_inserter< std::vector< std::string > >( extTokens ) );
        //std::copy( std::istream_iterator< std::string >( splitExtentions ), std::istream_iterator< std::string >( ),
        //           std::ostream_iterator< std::string >( std::cout, "\n" ) );

        std::cout << "Platform info [ " << m_numPlat++ << " ]" << std::endl;
        std::cout << std::setw( colWidth ) << "CL_PLATFORM_PROFILE : " << szPlatformProfile << std::endl;
        std::cout << std::setw( colWidth ) << "CL_PLATFORM_VERSION : " << szPlatformVersion << std::endl;
        std::cout << std::setw( colWidth ) << "CL_PLATFORM_NAME : " << szPlatformName << std::endl;
        std::cout << std::setw( colWidth ) << "CL_PLATFORM_VENDOR : " << szPlatformVendor<< std::endl;
        std::cout << std::setw( colWidth ) << "CL_PLATFORM_EXTENSIONS : " << std::endl;
        std::for_each( extTokens.begin( ), extTokens.end( ), printExtention );
        std::cout << std::endl;

        // For each device for this specific platform, print all the associated information
        std::vector< cl::Device > devices;
        bolt::cl::V_OPENCL( plat.getDevices( m_deviceType, &devices ), "Platform::getDevices() failed" );

        std::for_each( devices.begin( ), devices.end( ), printDeviceFunctor( 0 ) );

    }
};

void bolt::cl::control::printPlatforms( bool printDevices, cl_device_type deviceType )
{
    //  Query OpenCL for available platforms
    cl_int err = CL_SUCCESS;

    // Platform vector contains all available platforms on system
    std::vector< ::cl::Platform > platforms;
    bolt::cl::V_OPENCL( ::cl::Platform::get( &platforms ), "Platform::get() failed" );

    std::for_each( platforms.begin( ), platforms.end( ), printPlatformFunctor( 0, deviceType ) );
}

void bolt::cl::control::printPlatformsRange( std::vector< ::cl::Platform >::iterator begin, std::vector< ::cl::Platform >::iterator end, 
                                            bool printDevices, cl_device_type deviceType )
{
    //  Query OpenCL for available platforms
    std::for_each( begin, end, printPlatformFunctor( 0, deviceType ) );
}

::cl::CommandQueue bolt::cl::control::getDefaultCommandQueue( )
{
    //  Query OpenCL for available platforms
    cl_int err = CL_SUCCESS;

    // Platform vector contains all available platforms on system
    std::vector< ::cl::Platform > platforms;
    bolt::cl::V_OPENCL( ::cl::Platform::get( &platforms ), "Platform::get() failed" );

    //  If Bolt detects 0 platforms, let the default OpenCL runtime handle this
    if( platforms.empty( ) )
        return ::cl::CommandQueue::getDefault( );

    //  Find all AMD platforms
    std::vector< ::cl::Platform > amdPlatforms;
    for( std::vector< ::cl::Platform >::iterator clPlatIter = platforms.begin(); clPlatIter != platforms.end( ); ++clPlatIter )
    {
        std::string szPlatformVendor = clPlatIter->getInfo< CL_PLATFORM_VENDOR >( &err );
        bolt::cl::V_OPENCL( err, "Platform::getInfo< CL_PLATFORM_VENDOR > failed" );

        if( szPlatformVendor.find( "Advanced Micro Devices, Inc." ) != std::string::npos )
        {
            amdPlatforms.push_back( *clPlatIter );
        }
    }

    //  No AMD platforms available to choose from; allow OpenCL runtime to choose for us
    if( amdPlatforms.empty( ) )
    {
        return ::cl::CommandQueue::getDefault( );
    }

    //  Choose the OpenCL device that has the greatest amount of available memory
    //std::max_element( amdPlatforms.begin( ), amdPlatforms.end( ), chooseDevice( CL_DEVICE_GLOBAL_MEM_SIZE, ::bolt::cl::max< cl_ulong >( ) ) );
    cl_ulong maxDeviceMemory = 0;
    cl_uint maxMaxClock = 0;
    cl_uint maxMaxUnits = 0;
    ::cl::Device boltDevice;
    for( std::vector< ::cl::Platform >::iterator amdPlatIter = amdPlatforms.begin( ); amdPlatIter != amdPlatforms.end( ); ++amdPlatIter )
    {
        //  We don't want to pick an AMD CPU device over another vendors GPU device, so filter only GPU devices
        std::vector< ::cl::Device > amdDevices;
        bolt::cl::V_OPENCL( amdPlatIter->getDevices( CL_DEVICE_TYPE_GPU, &amdDevices ), "Platform::getDevices() failed" );

        //  If there are no AMD GPU devices, revert to letting the OpenCL runtime pick the best device
        if( amdDevices.empty( ) )
        {
            return ::cl::CommandQueue::getDefault( );
        }

        for( std::vector< ::cl::Device >::iterator amdDevIter = amdDevices.begin( ); amdDevIter != amdDevices.end( ); ++amdDevIter )
        {
            cl_ulong devDeviceMemory = amdDevIter->getInfo< CL_DEVICE_GLOBAL_MEM_SIZE >( &err );
            bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_GLOBAL_MEM_SIZE > failed" );

            cl_uint devMaxClock = amdDevIter->getInfo< CL_DEVICE_MAX_CLOCK_FREQUENCY >( &err );
            bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_MAX_CLOCK_FREQUENCY > failed" );

            cl_uint devMaxUnits = amdDevIter->getInfo< CL_DEVICE_MAX_COMPUTE_UNITS >( &err );
            bolt::cl::V_OPENCL( err, "Device::getInfo< CL_DEVICE_MAX_COMPUTE_UNITS > failed" );

            cl_uint maxWorkPot = maxMaxClock * maxMaxUnits;
            cl_uint devWorkPot = devMaxClock * devMaxUnits;

            //  Multiply the CU by the frequency, to arrive at a estimate of work potential
            if( devWorkPot > maxWorkPot )
            {
                boltDevice = *amdDevIter;
                maxDeviceMemory = devDeviceMemory;
                maxMaxClock = devMaxClock;
                maxMaxUnits = devMaxUnits;
            }
            else if( devWorkPot == maxWorkPot )
            {
                //  Tie breakers go to the device with the most memory
                if( devDeviceMemory > maxDeviceMemory )
                {
                    boltDevice = *amdDevIter;
                    maxDeviceMemory = devDeviceMemory;
                    maxMaxClock = devMaxClock;
                    maxMaxUnits = devMaxUnits;
                }
            }
        }
    }

    //  If for some reason no device was picked in the loop above, revert to OpenCL runtime choosing
    if( maxDeviceMemory == 0 )
    {
        return ::cl::CommandQueue::getDefault( );
    }

    //  Create a ::cl::CommandQueue for the device we chose
    ::cl::Context boltContext( boltDevice );
    ::cl::CommandQueue boltQueue( boltContext, boltDevice );

    return boltQueue;

}