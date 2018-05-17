
import processing.serial.*;
import processing.opengl.*;

Serial myPort;
String val;

float X;
float Y;
float Z;

float jX;
float jY;

float bZ;
float bC;

// Kalman vars
float P1 = 0;
float Q1 = .1;
float R1 = 0.3;
float K1 = 0;
float X1 = 0;

float P2 = 0;
float Q2 = 0.1;
float R2 = 0.3;
float K2 = 0;
float X2 = 0;

void setup() {
    size(450, 450, P3D);

    String portName = Serial.list()[1];

    myPort = new Serial(this, portName, 9600);
}

void draw() {

    if (myPort.available() > 0) {

        val = myPort.readStringUntil('\n');

        float[] tmp;
        try{
            tmp = float(val.split(","));
        } catch(Exception e) {
            return;
        }
        if (tmp.length != 7) {
            return;
        }

        jX = tmp[0];
        jY = tmp[1];
        X = tmp[2];
        Y = tmp[3];
        Z = tmp[4];
        bZ = tmp[5];
        bC = tmp[6];
    } else {
        return;
    }

    // Kalman 1 - pitch
    float Xtmp = X1;
    float Ptmp = P1 + Q1;
    K1 = Ptmp / (Ptmp + R1);
    X1 = Xtmp + K1 * (atan2(Y, Z) - Xtmp);
    P1 = (1 - K1) * Ptmp;
    X1 = atan2(Y, Z);

    // Kalman 2 - roll
    Xtmp = X2;
    Ptmp = P2 + Q2;
    K2 = Ptmp / (Ptmp + R2);
    X2 = Xtmp + K2 * (atan2(X, Z) - Xtmp);
    P2 = (1 - K2) * Ptmp;
    X2 = atan2(X, Z);

    background(60, 60, 60);
    lights();

    fill(245, 10, 10);

    pushMatrix();
    translate(225, 300, 0);
    rotateX(1.3 + (X1 - 0));
    rotateY((X2 - 0));
    drawCylinder(20, 40, 80, 80);
    popMatrix();

}

void drawCylinder(int sides, float r1, float r2, float h) {
    float angle = 360 / sides;
    float halfHeight = h / 2;

    // draw top of the tube
    beginShape();
    for (int i = 0; i < sides; i++) {
        float x = cos(radians(i * angle)) * r1;
        float y = sin(radians(i * angle)) * r1;
        vertex(x, y, 0);
    }
    endShape(CLOSE);

    // draw bottom of the tube
    beginShape();
    for (int i = 0; i < sides; i++) {
        float x = cos(radians(i * angle)) * r2;
        float y = sin(radians(i * angle)) * r2;
        vertex(x, y, 2 * halfHeight);
    }
    endShape(CLOSE);

    // draw sides
    beginShape(TRIANGLE_STRIP);
    for (int i = 0; i < sides + 1; i++) {
        float x1 = cos(radians(i * angle)) * r1;
        float y1 = sin(radians(i * angle)) * r1;
        float x2 = cos(radians(i * angle)) * r2;
        float y2 = sin(radians(i * angle)) * r2;
        vertex(x1, y1, 0);
        vertex(x2, y2, 2 * halfHeight);
    }
    endShape(CLOSE);
}
