#include "potential_armor.h"
#include "potential_light.h"
#include <stdio.h>
#include <thread>

using namespace cv;
using namespace std;

PotentialArmor::PotentialArmor() {}

PotentialArmor::PotentialArmor(PotentialLight one, PotentialLight two) {
    light1 = one;
    light2 = two;
};

bool PotentialArmor::matchAngle() {
    float difference = light1.getAngle() - light2.getAngle();
    return abs(difference) < 20;
}

bool PotentialArmor::matchArea() {
    float area1 = light1.getHeight() * light1.getWidth();
    float area2 = light2.getHeight() * light2.getWidth();

    float prop_areas = area1 / area2;

    return (prop_areas < 2 && prop_areas > 0.5);
}

bool PotentialArmor::matchHeight() {
    float height1 = light1.getHeight();
    float height2 = light2.getHeight();

    float prop_heights = height1 / height2;

    return (prop_heights < 1.5 && prop_heights > 0.5);
}

bool PotentialArmor::matchY() {
    // top and bottom y pos of each lightbar with buffer offset
    // added 10 for buffer
    float top1 = light1.getTop().y - 10;
    float bottom1 = light1.getBottom().y + 10;

    float top2 = light2.getTop().y - 10;
    float bottom2 = light2.getBottom().y + 10;

    // is the top of two in the y-bounds of one?
    if (top2 <= bottom1 && top2 >= top1) {
        return true;
    }
    // is the bottom of two in the y-bounds of one?
    else if (bottom2 <= bottom1 && bottom2 >= top1) {
        return true;
    }
    // is the top of one in the y-bounds of two?
    else if (top1 <= bottom2 && top1 >= top2) {
        return true;
    }
    // is the top of one in the y-bounds of two?
    else if (bottom1 <= bottom2 && bottom1 >= top2) {
        return true;
    }

    return false;
}

cv::Point2f PotentialArmor::getCenter() {
    return (light1.getCenter() + light2.getCenter()) / 2;
}

vector<Point2f> PotentialArmor::getCorners() {
    vector<Point2f> corners{ light1.getTop(), light2.getTop(),
        light1.getBottom(), light2.getBottom() };
    return corners;
}

tuple<double, double> PotentialArmor::getDistance() {
    // get top and bottom width distances for each pair of lights passed in 
    double top_distance = sqrt(pow((light1.getTop().x - light2.getTop().x), 2) + pow((light1.getTop().y - light2.getTop().y), 2));
    double bot_distance = sqrt(pow((light1.getBottom().x - light2.getBottom().x), 2) + pow((light1.getBottom().y - light2.getBottom().y), 2));
    return make_tuple(top_distance, bot_distance);
}

// new function added 
bool PotentialArmor::checkProportion() {
    // set up variables - get distances and set up max containers for each
    double max_height = light1.getHeight();
    tuple<double, double> distances = getDistance();
    double distance1 = get<0>(distances);
    double distance2 = get<1>(distances);
    double max_distance = distance1;

    // check up maxes for both calculated distance and width
    if (max_height < light2.getHeight()) {
        max_height = light2.getHeight();
    }

    if (max_distance < distance2) {
        max_distance = distance2;
    }

    // write proportion checks - checks to see if the height is within 3 times max distance
    // and checks to make sure that max height is less than max distance (wouldn't make sense for an
    // armor plate to have a larger height than it's width, but might want to double check this)
    if (max_height > max_distance) {
        return false;
    }

    return true;
}

ArmorState PotentialArmor::validate() {
    if (!matchAngle()) {
        return ArmorState::ANGLE_ERROR;
    }
    else if (!matchHeight()) {
        return ArmorState::HEIGHT_ERROR;
    }
    else if (!matchY()) {
        return ArmorState::Y_ERROR;
    }
    else if (!checkProportion()) { 
        return ArmorState::PROP_ERROR;
    }
    else {
        return ArmorState::NO_ERROR;
    }
}
