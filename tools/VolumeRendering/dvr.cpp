#include <GetSetGui/GetSetGui.h>
#include <LibSimple/QVolumeRenderer.h>

GetSetGui::Application g_app("VolumeRendering");

SimpleVR::QVolumeRenderer       *g_dvr=0x0;

void load_volume()
{
	if (g_dvr) delete g_dvr;
	std::string file=GetSet<>("Volume Rendering/Input Volume");
	g_dvr=new SimpleVR::QVolumeRenderer(file);
	g_dvr->show();
	g_dvr->setIsoValue(GetSet<double>("Volume Rendering/Iso Value"));
	g_dvr->setShaded(GetSet<bool>("Volume Rendering/Shaded"));
	g_dvr->setSamplesPerVoxel(GetSet<double>("Volume Rendering/Samples Per Voxel"));
}

void gui(const GetSetInternal::Node& node)
{
	if (node.name=="Input Volume")
		load_volume();
	if (node.name=="Iso Value")
		if (g_dvr) g_dvr->setIsoValue(GetSet<double>("Volume Rendering/Iso Value"));
	if (node.name=="Shaded")
		if (g_dvr) g_dvr->setShaded(GetSet<bool>("Volume Rendering/Shaded"));
	if (node.name=="Samples Per Voxel")
		if (g_dvr) g_dvr->setSamplesPerVoxel(GetSet<double>("Volume Rendering/Samples Per Voxel"));

	g_app.saveSettings();
}

int main(int argc, char ** argv)
{
	GetSetGui::File("Volume Rendering/Input Volume").setExtensions("Meta Image Header (*.mhd);;All Files (*)")="./data/card/card.mhd";
	GetSet<double>("Volume Rendering/Iso Value")=.4;
	GetSet<bool>("Volume Rendering/Shaded")=true;
	GetSet<double>("Volume Rendering/Samples Per Voxel")=1.5;

	g_app.init(argc,argv,gui);
	load_volume();
	return g_app.exec();
}
