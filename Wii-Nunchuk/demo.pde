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

void setup() {
    size(450, 450, OPENGL);

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

    background(0, 128, 255);
    lights();

    fill(255, 128, 0);

    pushMatrix();
    translate(225, 300, 0);
    rotateX(1.3 + atan2(Y, Z));
    rotateY(atan2(X, Z));
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
