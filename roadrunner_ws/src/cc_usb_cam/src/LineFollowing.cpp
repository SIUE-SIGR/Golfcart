/* 
author: Cathy Casey
last modified: 28 April 2015
this file uses the usb cameras for line following
*/

#include "LineFollowing.h"
#include <string>
#include <stdlib>
#include <cmath>

string LineFollowing::LinePosition()
{
  Blob currBlob;

  //blob x position is above 320 (on the right side of the image)
  if(currBlob.x > 320)
  {
    linePos = "right";
    return linePos;
  }
  else
  {
    linePos = "left";
    return linePos;
  }
  return "no line?";
}

string LineFollowing::ComparePositions()
{
  Blob blob1;
  Blob blob2;
  LinePosition();
  if(blob1.linePos == "left" && blob2.linePos == "right")
  {
    //blobs on both sides
    float xDiff = abs(blob1.x - blob2.x);
  }

}



















