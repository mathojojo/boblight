/*
 * boblight
 * Copyright (C) Bob  2009 
 * 
 * boblight is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * boblight is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CBOBLIGHT
#define CBOBLIGHT

#include <string>
#include <vector>

#include "util/tcpsocket.h"
#include "util/messagequeue.h"
#include "util/clock.h"

#include "libboblight.h"

//used to define a polygon
struct boblight_point
{
  int x;
  int y;
};

class CLight
{
  public:
    CLight();

    std::string m_name;
    float       m_speed;
    bool        m_interpolation;
    bool        m_use;

    float       m_value;
    float       m_saturation;
    float       m_valuerange[2];
    int         m_threshold;

    int         m_rgbd[4];
    void        GetRGB(float* rgb);

    std::vector<boblight_point> m_area;
    std::vector<unsigned char>  m_scanarea;
    int                         m_width;
    int                         m_height;
};  

class CBoblight
{
  public:
    int         Connect(const char* address, int port, int usectimeout);
    const char* GetError()                    { return m_error.c_str(); }

    int         GetNrLights()                 { return m_lights.size(); }
    const char* GetLightName    (int lightnr);

    int         SetPriority     (int priority);
    int         SetSpeed        (int lightnr, float speed);
    int         SetInterpolation(int lightnr, int on);
    int         SetUse          (int lightnr, int on);
    int         SetValue        (int lightnr, float value);
    int         SetSaturation   (int lightnr, float saturation);
    int         SetValueRange   (int lightnr, float* valuerange);
    int         SetThreshold    (int lightnr, int threshold);
    void        SetScanRange    (int width,   int height);

    void        AddPixel(int lightnr, int* rgb);
    void        AddPixel(int* rgb, int x, int y);

    int         SendRGB();
    int         Ping();
    
  private:
    CTcpClientSocket m_socket;
    std::string      m_address;
    int              m_port;
    std::string      m_error;
    CMessageQueue    m_messagequeue;
    CClock           m_clock;
    int              m_usectimeout;

    bool             ReadDataToQueue();
    bool             WriteDataToSocket(std::string strdata);
    bool             ParseWord(CMessage& message, std::string wordtocmp);
    bool             ParseWord(CMessage& message, std::string wordtocmp, std::string readword);
    bool             ParseLights(CMessage& message);
    bool             CheckLightExists(int lightnr, bool printerror = true);

    void             DrawPolygon(CLight& light);
    void             FillPolygon(CLight& light);
    void             TransformPoint(int* point, int width, int height);
    bool             IsPointInPolygon(CLight& light, int x, int y);

    std::vector<CLight> m_lights;
};

#endif //CBOBLIGHT