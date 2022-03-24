#include <PhysicalDevice.hpp>

#define _WIN32_DCOM
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include <Rpcdce.h>
#pragma comment(lib, "wbemuuid.lib")

#include <d3d9.h>

#include <windows.h>
#include <winuser.h>

#include <GL/glew.h>

namespace OCRA::PhysicalDevice
{
Limits GetPhysicalDeviceLimitsGL()
{
    glGetInteger(GL_MAX_TEXTURE_SIZE, &properties.limits.maxImageDimension1D);
    glGetInteger(GL_MAX_RECTANGLE_TEXTURE_SIZE, &properties.limits.maxImageDimension2D);
    glGetInteger(GL_MAX_3D_TEXTURE_SIZE, &properties.limits.maxImageDimension3D);
    glGetInteger(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &properties.limits.maxImageDimensionCube);
    glGetInteger(GL_MAX_ARRAY_TEXTURE_LAYERS, &properties.limits.maxImageArrayLayers);
    glGetInteger(GL_MAX_TEXTURE_BUFFER_SIZE, &properties.limits.maxTexelBufferElements);
    glGetInteger(GL_MAX_UNIFORM_BLOCK_SIZE, &properties.limits.maxUniformBufferRange);
    glGetInteger(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &properties.limits.maxStorageBufferRange);
    glGetInteger(    , &properties.limits.maxPushConstantsSize);
    glGetInteger(    , &properties.limits.maxMemoryAllocationCount);
    glGetInteger(    , &properties.limits.maxSamplerAllocationCount);
    glGetInteger(    , &properties.limits.bufferImageGranularity);
    glGetInteger(    , &properties.limits.sparseAddressSpaceSize);
    glGetInteger(    , &properties.limits.maxBoundDescriptorSets);

    glGetInteger(    , &properties.limits.maxPerStageDescriptorSamplers);
    glGetInteger(    , &properties.limits.maxPerStageDescriptorUniformBuffers);
    glGetInteger(    , &properties.limits.maxPerStageDescriptorStorageBuffers);
    glGetInteger(    , &properties.limits.maxPerStageDescriptorSampledImages);
    glGetInteger(    , &properties.limits.maxPerStageDescriptorStorageImages);
    glGetInteger(    , &properties.limits.maxPerStageDescriptorInputAttachments);
    glGetInteger(    , &properties.limits.maxPerStageResources);

    glGetInteger(    , &properties.limits.maxDescriptorSetSamplers);
    glGetInteger(    , &properties.limits.maxDescriptorSetUniformBuffers);
    glGetInteger(    , &properties.limits.maxDescriptorSetUniformBuffersDynamic);
    glGetInteger(    , &properties.limits.maxDescriptorSetStorageBuffers);
    glGetInteger(    , &properties.limits.maxDescriptorSetStorageBuffersDynamic);
    glGetInteger(    , &properties.limits.maxDescriptorSetSampledImages);
    glGetInteger(    , &properties.limits.maxDescriptorSetStorageImages);
    glGetInteger(    , &properties.limits.maxDescriptorSetInputAttachments);

    glGetInteger(GL_MAX_VERTEX_ATTRIBS, &properties.limits.maxVertexInputAttributes);
    glGetInteger(GL_MAX_VERTEX_ATTRIB_BINDINGS, &properties.limits.maxVertexInputBindings);
    glGetInteger(    , &properties.limits.maxVertexInputAttributeOffset);
    glGetInteger(    , &properties.limits.maxVertexInputBindingStride);
    glGetInteger(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &properties.limits.maxVertexOutputComponents);

    glGetInteger(    , &properties.limits.maxTessellationGenerationLevel);
    glGetInteger(    , &properties.limits.maxTessellationPatchSize);
    glGetInteger(    , &properties.limits.maxTessellationControlPerVertexInputComponents);
    glGetInteger(    , &properties.limits.maxTessellationControlPerVertexOutputComponents);
    glGetInteger(    , &properties.limits.maxTessellationControlPerPatchOutputComponents);
    glGetInteger(    , &properties.limits.maxTessellationControlTotalOutputComponents);
    glGetInteger(    , &properties.limits.maxTessellationEvaluationInputComponents);
    glGetInteger(    , &properties.limits.maxTessellationEvaluationOutputComponents);

    glGetInteger(    , &properties.limits.maxGeometryShaderInvocations);
    glGetInteger(GL_MAX_GEOMETRY_INPUT_COMPONENTS, &properties.limits.maxGeometryInputComponents);
    glGetInteger(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS, &properties.limits.maxGeometryOutputComponents);
    glGetInteger(    , &properties.limits.maxGeometryOutputVertices);
    glGetInteger(    , &properties.limits.maxGeometryTotalOutputComponents);

    glGetInteger(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &properties.limits.maxFragmentInputComponents);
    glGetInteger(    , &properties.limits.maxFragmentOutputAttachments);
    glGetInteger(    , &properties.limits.maxFragmentDualSrcAttachments);
    glGetInteger(    , &properties.limits.maxFragmentCombinedOutputResources);

    glGetInteger(    , &properties.limits.maxComputeSharedMemorySize);
    glGetInteger(GL_MAX_COMPUTE_WORK_GROUP_COUNT, &properties.limits.maxComputeWorkGroupCount);
    glGetInteger(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &properties.limits.maxComputeWorkGroupInvocations);
    glGetInteger(GL_MAX_COMPUTE_WORK_GROUP_SIZE, &properties.limits.maxComputeWorkGroupSize);
    glGetInteger(    , &properties.limits.subPixelPrecisionBits);
    glGetInteger(    , &properties.limits.subTexelPrecisionBits);
    glGetInteger(    , &properties.limits.mipmapPrecisionBits);
    glGetInteger(    , &properties.limits.maxDrawIndexedIndexValue);
    glGetInteger(    , &properties.limits.maxDrawIndirectCount);
    glGetInteger(    , &properties.limits.maxSamplerLodBias);
    glGetInteger(    , &properties.limits.maxSamplerAnisotropy);
    glGetInteger(    , &properties.limits.maxViewports);
    glGetInteger(    , &properties.limits.maxViewportDimensions);
    glGetInteger(    , &properties.limits.viewportBoundsRange);
    glGetInteger(    , &properties.limits.viewportSubPixelBits);
    glGetInteger(    , &properties.limits.minMemoryMapAlignment);
    glGetInteger(    , &properties.limits.minTexelBufferOffsetAlignment);
    glGetInteger(    , &properties.limits.minUniformBufferOffsetAlignment);
    glGetInteger(    , &properties.limits.minStorageBufferOffsetAlignment);
    glGetInteger(    , &properties.limits.minTexelOffset);
    glGetInteger(    , &properties.limits.maxTexelOffset);
    glGetInteger(    , &properties.limits.minTexelGatherOffset);
    glGetInteger(    , &properties.limits.maxTexelGatherOffset);
    glGetInteger(    , &properties.limits.minInterpolationOffset);
    glGetInteger(    , &properties.limits.maxInterpolationOffset);
    glGetInteger(    , &properties.limits.subPixelInterpolationOffsetBits);
    glGetInteger(    , &properties.limits.maxFramebufferWidth);
    glGetInteger(    , &properties.limits.maxFramebufferHeight);
    glGetInteger(    , &properties.limits.maxFramebufferLayers);
    glGetInteger(    , &properties.limits.framebufferColorSampleCounts);
    glGetInteger(    , &properties.limits.framebufferDepthSampleCounts);
    glGetInteger(    , &properties.limits.framebufferStencilSampleCounts);
    glGetInteger(GL_MAX_FRAMEBUFFER_SAMPLES, &properties.limits.framebufferNoAttachmentsSampleCounts);
    glGetInteger(    , &properties.limits.maxColorAttachments);
    glGetInteger(GL_MAX_COLOR_TEXTURE_SAMPLES, &properties.limits.sampledImageColorSampleCounts);
    glGetInteger(GL_MAX_INTEGER_SAMPLES, &properties.limits.sampledImageIntegerSampleCounts);
    glGetInteger(GL_MAX_DEPTH_TEXTURE_SAMPLES, &properties.limits.sampledImageDepthSampleCounts);
    glGetInteger(    , &properties.limits.sampledImageStencilSampleCounts);
    glGetInteger(    , &properties.limits.storageImageSampleCounts);
    glGetInteger(    , &properties.limits.maxSampleMaskWords);
    glGetInteger(    , &properties.limits.timestampComputeAndGraphics);
    glGetInteger(    , &properties.limits.timestampPeriod);
    glGetInteger(    , &properties.limits.maxClipDistances);
    glGetInteger(    , &properties.limits.maxCullDistances);
    glGetInteger(    , &properties.limits.maxCombinedClipAndCullDistances);
    glGetInteger(    , &properties.limits.discreteQueuePriorities);
    glGetInteger(    , &properties.limits.pointSizeRange);
    glGetInteger(    , &properties.limits.lineWidthRange);
    glGetInteger(    , &properties.limits.pointSizeGranularity);
    glGetInteger(    , &properties.limits.lineWidthGranularity);
    glGetInteger(    , &properties.limits.strictLines);
    glGetInteger(    , &properties.limits.standardSampleLocations);
    glGetInteger(    , &properties.limits.optimalBufferCopyOffsetAlignment);
    glGetInteger(    , &properties.limits.optimalBufferCopyRowPitchAlignment);
    glGetInteger(    , &properties.limits.nonCoherentAtomSize);
    properties.sparseProperties = 0;
}
Properties GetPhysicalDeviceWGL()
{
    //device context handle
    HDC     hdc   = GetDC(nullptr); //if hwnd is null, GetDC retrieves the DC for the entire screen 
    //gl render context handle
    HGLRC   hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);
    if (glewInit() != GLEW_OK) throw std::runtime_error("Cound not initialize GLEW");

    //DX9 to the rescue !
    auto pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    D3DADAPTER_IDENTIFIER9  d3dIdentifier;
    D3DCAPS9                d3dCaps;
    D3DDISPLAYMODE          d3dDisplayMode;
    pD3D->GetAdapterIdentifier(
        D3DADAPTER_DEFAULT,
        0,
        &d3dIdentifier);
    pD3D->GetDeviceCaps(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        &d3dCaps);
    pD3D->GetAdapterDisplayMode(
        D3DADAPTER_DEFAULT,
        &d3dDisplayMode);
    pD3D->Release();
    Properties properties;
    properties.apiVersion = 430;
    properties.driverVersion = d3dIdentifier.DriverVersion;
    properties.vendorID = d3dIdentifier.VendorId;
    properties.deviceID = d3dIdentifier.DeviceId;
    properties.deviceType = Type::Other;
    properties.deviceName = d3dIdentifier.DeviceName;
    properties.pipelineCacheUUID = 0;
    properties.limits = GetLimits();

    wglMakeCurrent(hdc, nullptr);
    wglDeleteContext(hglrc);
    ReleaseDC(hdc);
    return properties;
}
int GetPhysicalDeviceWMI()
{
    HRESULT hres;

    // Step 1: --------------------------------------------------
    // Initialize COM. ------------------------------------------

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        std::cout << "Failed to initialize COM library. Error code = 0x"
            << std::hex << hres << std::endl;
        return 1;                  // Program has failed.
    }
    hres = CoInitializeSecurity(
        NULL,
        -1,                          // COM authentication
        NULL,                        // Authentication services
        NULL,                        // Reserved
        RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
        RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
        NULL,                        // Authentication info
        EOAC_NONE,                   // Additional capabilities 
        NULL                         // Reserved
    );


    if (FAILED(hres))
    {
        std::cout << "Failed to initialize security. Error code = 0x"
            << std::hex << hres << std::endl;
        CoUninitialize();
        return 1;                    // Program has failed.
    }

    // Step 3: ---------------------------------------------------
    // Obtain the initial locator to WMI -------------------------

    IWbemLocator* pLoc = NULL;

    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLoc);

    if (FAILED(hres))
    {
        std::cout << "Failed to create IWbemLocator object."
            << " Err code = 0x"
            << std::hex << hres << std::endl;
        CoUninitialize();
        return 1;                 // Program has failed.
    }

    // Step 4: -----------------------------------------------------
    // Connect to WMI through the IWbemLocator::ConnectServer method

    IWbemServices* pSvc = NULL;

    // Connect to the root\cimv2 namespace with
    // the current user and obtain pointer pSvc
    // to make IWbemServices calls.
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
        NULL,                    // User name. NULL = current user
        NULL,                    // User password. NULL = current
        0,                       // Locale. NULL indicates current
        NULL,                    // Security flags.
        0,                       // Authority (for example, Kerberos)
        0,                       // Context object 
        &pSvc                    // pointer to IWbemServices proxy
    );

    if (FAILED(hres))
    {
        std::cout << "Could not connect. Error code = 0x"
            << std::hex << hres << std::endl;
        pLoc->Release();
        CoUninitialize();
        return 1;                // Program has failed.
    }

    std::cout << "Connected to ROOT\\CIMV2 WMI namespace" << std::endl;


    // Step 5: --------------------------------------------------
    // Set security levels on the proxy -------------------------

    hres = CoSetProxyBlanket(
        pSvc,                        // Indicates the proxy to set
        RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
        RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
        NULL,                        // Server principal name 
        RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
        RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
        NULL,                        // client identity
        EOAC_NONE                    // proxy capabilities 
    );

    if (FAILED(hres))
    {
        std::cout << "Could not set proxy blanket. Error code = 0x"
            << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;               // Program has failed.
    }

    // Step 6: --------------------------------------------------
    // Use the IWbemServices pointer to make requests of WMI ----

    // For example, get the name of the operating system
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM Win32_VideoController"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres))
    {
        std::cout << "Query for operating system name failed."
            << " Error code = 0x"
            << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;               // Program has failed.
    }

    // Step 7: -------------------------------------------------
    // Get the data from the query in step 6 -------------------

    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator)
    {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
            &pclsObj, &uReturn);

        if (0 == uReturn)
        {
            break;
        }

        VARIANT vtProp;

        // Get the value of the Name property
        hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
        std::cout << " GPU Name : " << vtProp.bstrVal << std::endl;
        VariantClear(&vtProp);

        pclsObj->Release();
    }

    // Cleanup
    // ========

    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();

    return 0;   // Program successfully completed.
}
struct Impl
{
	Impl()
	{

	}
	Properties properties;
};
Handle Create()
{
	return Handle(new Impl());
}
const Properties& GetProperties(const Handle& a_PhysicalDevice)
{
	return a_PhysicalDevice->properties;
}
}