/**
////////////////////////////////////////////////////////////////////////////////
Procedure to initialize :
////////////////////////////////////////////////////////////////////////////////
Create Window
Create Device using Window

////////////////////////////////////////////////////////////////////////////////
Procedure to draw :
////////////////////////////////////////////////////////////////////////////////
Create Graphic Pipeline
Create Vertex Buffers
Create Command Buffer
Create Render Pass

Begin recording Command Buffer
	Begin recording Render Pass
		Bind Pipeline
		Bind Vertex Buffers
		Bind Index Buffer
		Bind Descriptor Sets
		Set Drawing Command
	End recording Render Pass
End recording Command Buffer
*/

#include <Instance.hpp>

using namespace OCRA;

int main()
{
	Instance::Handle instance;
	{
		Instance::Info instanceInfo;
		instanceInfo.applicationInfo.name = "Test";
		instance = Instance::Create(instanceInfo);
	}
	auto physicalDevices = Instance::EnumeratePhysicalDevices(instance);
}