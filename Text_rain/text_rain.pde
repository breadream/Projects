
import processing.video.*;

// Global variables for input selection and data
String[] cameras;
PFont f; 
Capture cam;
PImage mov;
PImage mov_reflect;
PImage inputImage;
boolean inputMethodSelected = false;
boolean debugModeSelected = false;
int startTime;
int frame;
// input string variable 
String inputLine = "Vale, puedes quedarte a mi lado, siempre que no hables sobre la tempertura. Je pense que ce vin a déjà ete bu que";
// position variables for each character
int [] poX = new int[inputLine.length()];
int [] poY = new int[inputLine.length()];
int [] poX_backup = new int[inputLine.length()];
int threshold;
int elapsed_time;



void loadFrame() {
  int newFrame = 1 + (millis() - startTime)/100; // get new frame every 0.1 sec
  if (newFrame == frame)
    return;
  frame = newFrame;
  String movieName = "TextRainInput";
  String filePath = movieName + "/" + nf(frame, 3) + ".jpg";
  mov = loadImage(filePath);
  if (mov == null) {
    startTime = millis();
    loadFrame();
  }
}


void setup() {
  size(1280, 720);  
  inputImage = createImage(width, height, RGB);
  mov_reflect = createImage(width, height, RGB);
  noStroke();
  f = loadFont("YuKyo_Yoko-Medium-25.vlw");
  textFont(f);

  // initialize the starting x position 
  poX[0] = 5; 
  // initialize the interval between characters 
  for (int i=1; i < inputLine.length(); i++) {
    poX[i] = poX[i-1] + (int)textWidth(inputLine.charAt(i-1));
  }
  // make a copy of the original X position values 
  for (int i = 0; i < inputLine.length(); i++) {
    poX_backup[i] = poX[i];
  }
  // initialize the threshold value as 128 
  threshold = 128;
  elapsed_time = millis();
}



void draw() {
  // When the program first starts, draw a menu of different options for which camera to use for input
  // The input method is selected by pressing a key 0-9 on the keyboard
  if (!inputMethodSelected) {
    cameras = Capture.list();
    int y=40;
    text("O: Offline mode, test with TextRainInput.mov movie file instead of live camera feed.", 20, y);
    y += 40; 
    for (int i = 0; i < min(9, cameras.length); i++) {
      text(i+1 + ": " + cameras[i], 20, y);
      y += 40;
    }
    return;
  }

  // This part of the draw loop gets called after the input selection screen, during normal execution of the program.
  // STEP 1.  Load an image, either from the image sequence or from a live camera feed. Store the result in the inputImage variable
  if (cam != null) {
    if (cam.available())
      cam.read();
    // apply blur filter 
    cam.filter(BLUR, 1.5);
    // video image in grayscale 
    cam.filter(GRAY);
    // This part flips the camera image
    for (int j = 0; j < cam.height; j++) {
      for (int i = 0; i < cam.width; i++) {
        color c = cam.pixels[i + cam.width * j];
        mov_reflect.pixels[(cam.width - i - 1) + cam.width * j] = c;
      }
    }

    // prompt the debug mode 
    if (debugModeSelected) {
      for (int y = 0; y < cam.height; y++) {
        for (int x = 0; x < cam.width; x++) {
          int index = x + y * cam.width;
          color c = mov_reflect.pixels[index];
          if (brightness(c) > threshold) {
            mov_reflect.pixels[index] = color(255);
          } else {
            mov_reflect.pixels[index] = color(0);
          }
        }
      }
    }

    mov_reflect.updatePixels();
    inputImage.copy(mov_reflect, 0, 0, cam.width, cam.height/2, 0, 0, inputImage.width, inputImage.height);
  } else if (mov != null) {
    loadFrame();
    // apply blur filter 
    mov.filter(BLUR, 1.5);
    // display the video image in grayscale 
    mov.filter(GRAY);
    // This part flips the video image 
    for (int j = 0; j < mov.height; j++) {
      for (int i = 0; i < mov.width; i++) {
        color c = mov.pixels[i + mov.width * j]; // current pixel
        mov_reflect.pixels[(mov.width - i - 1) + mov.width * j] = c; // update pixel
      }
    }

    // prompt the debug mode 
    if (debugModeSelected) {
      for (int y = 0; y < mov.height; y++) {
        for (int x = 0; x < mov.width; x++) {
          int index = x + y * mov.width;
          color c = mov_reflect.pixels[index];
          if (brightness(c) > threshold) {
            mov_reflect.pixels[index] = color(255);
          } else {
            mov_reflect.pixels[index] = color(0);
          }
        }
      }
    }
    mov_reflect.updatePixels();
    inputImage.copy(mov_reflect, 0, 0, mov.width, mov.height/2, 0, 0, inputImage.width, inputImage.height);
  }

  // Tip: This code draws the current input image to the screen
  set(0, 0, inputImage);
  
  // characters fall down the screen 
  for (int i =  0; i < inputLine.length(); i++) { 
    if ((poY[i] < 0) || (poY[i] > mov_reflect.height)) {
      poX[i] = poX_backup[i]; 
      poY[i] = 0;
    } else { // stop falling when character find a dark object determined by the threshold value
      poY[i] += int((brightness(get(poX[i], poY[i])) - threshold) * elapsed_time / random(elapsed_time*10, elapsed_time*15));
      elapsed_time = millis();
    }
  }
  // text rain flows down a slope  
  for (int i = 1; i < inputLine.length()-1; i++) {
    int below = int(brightness(get(poX[i], poY[i]+12)) - threshold);
    int left = int(brightness(get(poX[i]-(int)textWidth(inputLine.charAt(i)), poY[i])) - threshold);
    int right = int(brightness(get(poX[i]+(int)textWidth(inputLine.charAt(i)), poY[i])) - threshold);
    if ((left > 0) &&  (right < 0) && (below < 0)) {
      poX[i] -= 12;
      poY[i] += 12;
    } else if ((left < 0) && (right > 0) && (below < 0)) {
      poX[i] += 12;
      poY[i] += 12;
    }
  }
  // draw the text 
  for (int i = 0; i < inputLine.length(); i++) {
    fill(random(255), random(255), random(255));
    text(inputLine.charAt(i), poX[i], poY[i]);
  }
}


void keyPressed() {

  if (!inputMethodSelected) { 
    // If we haven't yet selected the input method, then check for 0 to 9 keypresses to select from the input menu
    if ((key >= '0') && (key <= '9')) { 
      int input = key - '0';
      if (input == 0) {
        println("Offline mode selected.");
        startTime = millis();
        loadFrame();
        inputMethodSelected = true;
      } else if ((input >= 1) && (input <= 9)) {
        println("Camera " + input + " selected.");           
        // The camera can be initialized directly using an element from the array returned by list():
        cam = new Capture(this, cameras[input-1]);
        cam.start();
        inputMethodSelected = true;
      }
    }
    return;
  }

  // This part of the keyPressed routine gets called after the input selection screen during normal execution of the program
  // Fill in your code to handle keypresses here..

  if (key == CODED) {
    if (keyCode == UP) {
      // increase the threshould value by 2
      if (threshold <= 255) {
        threshold += 2;
      }
    } else if (keyCode == DOWN) {
      // decrease the threshould value by 2
      if (threshold >= 0) {
        threshold -= 2;
      }
    }
  } else if (key == ' ') {
    // toggle the debugging view 
    debugModeSelected = !debugModeSelected;
  }
}
