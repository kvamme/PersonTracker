/* Copyright 2011 AIT Austrian Institute of Technology
*
* This file is part of OpenTLD.
*
* OpenTLD is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* OpenTLD is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with OpenTLD. If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "Trajectory.h"

#include <cstdio>

/**
* @author Clemens Korner
*/

using namespace std;

namespace tld
{

Trajectory::Trajectory() :
    m_length(0)
{
}

Trajectory::~Trajectory()
{
}

void Trajectory::init(std::size_t length)
{
        m_length = length;
        m_track_positions = vector<CvPoint>();
        m_track_colors = vector<CvScalar>();
}

void Trajectory::addPoint(CvPoint point, CvScalar color)
{
        size_t length = m_track_positions.size();

        // vectors aren't full
        if(length < m_length)
        {
                m_track_positions.push_back(point);
                m_track_colors.push_back(color);
        }
        else
        {
                // push element to the end
                m_track_positions.push_back(point);
                m_track_colors.push_back(color);

                // drop first element
                m_track_positions.erase(m_track_positions.begin());
                m_track_colors.erase(m_track_colors.begin());
        }
}

void Trajectory::drawTrajectory(IplImage * image)
{
        CvPoint tempPoint;
        bool needSecondPoint = false;
        for(size_t i = 0; i < m_track_positions.size(); i++)
        {
                // try to find 1. point of the line
                if((!needSecondPoint)&&(m_track_positions[i].x != -1)) {
                        tempPoint = m_track_positions[i];
                        needSecondPoint = true;
                // try to find 2. point of the line
                }
                else if(needSecondPoint&&(m_track_positions[i].x != -1))
                {
                        cvLine(image, tempPoint, m_track_positions[i], m_track_colors[i], 2);
                        tempPoint = m_track_positions[i];
                }
        }
}
bool Trajectory::compare()
{
        float xAvg = 0;
        float yAvg = 0;
        size_t current = m_track_positions.size();
	if(current <2) return false;
        for(size_t i = current-3; i < current-1; i++)
        {
                if(m_track_positions[i].x != -1)
                {
                        xAvg += m_track_positions[i].x;
                        yAvg += m_track_positions[i].y;
                }
                else return false;
        }
        xAvg = xAvg/2;
        yAvg = yAvg/2;
        if( (xAvg + 5 < m_track_positions[current].x)||( xAvg - 5 > m_track_positions[current].x)){
                return false;
        }
        if( (yAvg + 5 < m_track_positions[current].y)||( yAvg - 5 > m_track_positions[current].y)){
                return false;
        }
        return true;
}

}
