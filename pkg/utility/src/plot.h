#include <vector>
#include <fstream>
#include <string>
#include <iostream>

class Point
{
public:
	double x;
	double y;
};

class Plot
{
public:
	Plot(const std::string &httpFile);
	void setTitle(const std::string &title);
	void setSubTitle(const std::string &subtitle);
	void setXName(const std::string &xName);
	void setYName(const std::string &yName);
	void setXMin(double min);
	void setYMin(double min);
	void addSet(const std::string &setName, std::vector<Point> &dataSet);
	void draw();
private:
	std::ofstream fout;
	std::string title;
	std::string subtitle;
	std::string xName;
	std::string yName;
	double xMin;
	double yMin;
	bool setMin;
	std::vector<std::vector<Point> > dataSets;
	std::vector<std::string> setNames;
};

Plot::Plot(const std::string &httpFile)
{
	fout.open(httpFile.c_str());
	xMin = 0;
	yMin = 0;
	setMin = false;
}
void Plot::setTitle(const std::string &title)
{
	this->title = title;
}
void Plot::setSubTitle(const std::string &subtitle)
{
	this->subtitle = subtitle;
}
void Plot::setXName(const std::string &xName)
{
	this->xName = xName;
}
void Plot::setYName(const std::string &yName)
{
	this->yName = yName;
}
void Plot::setXMin(double min)
{
	xMin = min;
	setMin = true;
}
void Plot::setYMin(double min)
{
	yMin = min;
	setMin = true;
}
void Plot::addSet(const std::string &setName, std::vector<Point> &dataSet)
{
	setNames.push_back(setName);
	dataSets.push_back(dataSet);
}
void Plot::draw()
{
	if(dataSets.size() == 0)
	{
		std::cout << "**ERROR Plot: No data to plot" << std::endl;
		return;
	}
	double defaultXMin = dataSets[0][0].x;
	double defaultYMin = dataSets[0][0].y;
	for(unsigned i = 0 ; i < setNames.size() ; ++i)
		for(unsigned j = 0 ; j < dataSets[i].size() ; ++j){
			defaultXMin = std::min(dataSets[i][j].x , defaultXMin);
			defaultYMin = std::min(dataSets[i][j].y , defaultYMin);
		}
	
	fout << "<script src=\"http://code.jquery.com/jquery-1.9.1.min.js\"></script>" <<std::endl;
	fout << "<script src=\"http://code.highcharts.com/highcharts.js\"></script>" << std::endl;
	fout << "<script src=\"http://code.highcharts.com/modules/exporting.js\"></script>" << std::endl;
	
	fout << "<div id=\"container\" style=\"min-width: 310px; height: 400px; margin: 0 auto\"></div>" << std::endl;
	
	fout << "<script>" << std::endl;
	fout << "$(function () {" <<std::endl;
	fout << "\t$('#container').highcharts({" << std::endl;

	fout << "\t\tchart: {" << std::endl;
	fout << "\t\t\ttype: 'spline'," << std::endl;
	fout << "\t\t\tzoomType: 'y'" << std::endl;
	fout << "\t\t}," << std::endl;

	fout << "\t\ttitle: {" << std::endl;
	fout << "\t\t\ttext: '" << title << "'" << std::endl;
	fout << "\t\t}," << std::endl;

	fout << "\t\tsubtitle: {" << std::endl;
	fout << "\t\t\ttext: '" << subtitle << "'" << std::endl;
	fout << "\t\t}," << std::endl;

	fout << "\t\txAxis: {" << std::endl;
	fout << "\t\t\ttitle: {" << std::endl;
	fout << "\t\t\t\ttext: '" << xName << "'" << std::endl;
	fout << "\t\t\t}," << std::endl;
	double plotXMin = (setMin)? xMin:defaultXMin;
	double plotYMin = (setMin)? yMin:defaultYMin;
	fout << "\t\t\tmin: " << plotXMin << std::endl;
	fout << "\t\t}," << std::endl;

	fout << "\t\tyAxis: {" << std::endl;
	fout << "\t\t\ttitle: {" << std::endl;
	fout << "\t\t\t\ttext: '" << yName << "'" << std::endl;
	fout << "\t\t\t}," << std::endl;
	fout << "\t\t\tmin: " << plotYMin << std::endl;
	fout << "\t\t}," << std::endl;
	
	fout << "\t\ttooltip: {" << std::endl;
	fout << "\t\t\theaderFormat: "<<  "'<b>{series.name}</b><br>', " << std::endl;
	fout << "\t\t\tpointFormat: "<< "'x = {point.x:.2f}  y = {point.y:.2f} m'" << std::endl;
	fout << "\t\t}," << std::endl;
	

	fout << "\t\tseries: [{" <<std::endl;
	for(unsigned i = 0 ; i < setNames.size() ; ++i)
	{
		fout << "\t\t\tname: '" << setNames[i] << "' ," << std::endl;
		fout << "\t\t\tdata: [" << std::endl;
		for(unsigned j = 0 ; j < dataSets[i].size() ; ++j)
			fout << "\t\t\t\t[" << dataSets[i][j].x << ", " << dataSets[i][j].y << "]," <<std::endl;
		fout << "\t\t\t]" << std::endl;
		fout << "\t\t}" << std::endl;
		
		if(i != setNames.size()-1)
			fout << "\t\t, {" << std::endl;
	}
	fout << "\t\t]" << std::endl;
	fout << "\t});" << std::endl;
	fout << "});" << std::endl;
	
	
	fout << "</script>" << std::endl;

}
