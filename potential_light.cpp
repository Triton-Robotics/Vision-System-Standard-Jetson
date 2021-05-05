#include "potential_light.h"
#include <stdio.h>
#include <thread>
#include <string>

using namespace cv;
using namespace std;

PotentialLight::PotentialLight() {}

PotentialLight::PotentialLight(RotatedRect box) {
    light_box = box;

    Mat light_check = Mat(512, 640, CV_8UC3);

    Mat corners_matrix;
    boxPoints(light_box, corners_matrix);

    // get the corners from the RotatedRect
    vector<Point2f> unordered_corners(4);
    corners = unordered_corners;

    Point2f rect_points[4];
    light_box.points(rect_points);

    for (int i = 0; i < 4; i++)
        corners[i] = rect_points[i];

    // sort the corners based on y-value
    for (int i = 0; i < 4; i++) {
        Point2f current = corners[i];

        for (int j = i; j < 4; j++) {
            if (corners[j].y < current.y) {
                corners[i] = corners[j];
                corners[j] = current;
                current = corners[i];
            }
        }
    }
};

vector<Point2f> PotentialLight::getCorners() {
    return corners;
}

float PotentialLight::getWidth() {
    float delta_x = abs(corners[0].x - corners[1].x);
    float delta_y = abs(corners[0].y - corners[1].y);

    return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}

float PotentialLight::getHeight() {
    float delta_x = abs(corners[0].x - corners[2].x);
    float delta_y = abs(corners[0].y - corners[2].y);

    return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}

float PotentialLight::getAngle() {

    vector<Point2f> corners = getCorners();

    Point2f top = (corners[0] + corners[1]) / 2;
    Point2f bottom = (corners[2] + corners[3]) / 2;

    // need to go the other way, since origin point is at top left.
    float degrees = atan2(bottom.y - top.y, top.x - bottom.x) * 180 / CV_PI;

    return degrees;
}

Point2f PotentialLight::getTop() {
    return (corners[0] + corners[1]) / 2;
}

Point2f PotentialLight::getBottom() {
    return (corners[2] + corners[3]) / 2;
}

Point2f PotentialLight::getCenter() {
    return (corners[0] + corners[3]) / 2;
}

LightState PotentialLight::validate() {
    float angle = getAngle();
    // angle should be within valid range
    if (!(angle < 120 && angle > 60)) {
        return LightState::ANGLE_ERROR;
    }
    float width = getWidth();
    float height = getHeight();

    // ratio of width : height should be acceptable
    if (width > height || height > width * 10) {
        return LightState::RATIO_ERROR;
    }

    float area = width * height;
    // light should not be too big or small (100 might be a good value)
    if (area < 300) {
        return LightState::AREA_ERROR;
    }

    // passed all tests, return true
    return LightState::NO_ERROR;
}
