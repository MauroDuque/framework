#include <Arduino.h>
#include "config_file.h"
#include "wifimanager.h"

bool done = true;

// // Menu states
enum MenuState {
  MAIN_MENU,
  WIFICONFIG_MODE_MENU
};

MenuState currentMenu = MAIN_MENU;

String getPasswordFromSerial() {
  String password = "";
  while (true) {
    while (Serial.available() > 0) {
      char c = Serial.read();
      if (c == '\n' || c == '\r') {
        Serial.println(); // Print newline after password input
        return password;
      }
      if (c == '\b' || c == 127) { // Handle backspace
        if (password.length() > 0) {
          password.remove(password.length() - 1);
          Serial.print("\b \b"); // Move cursor back, print space, move cursor back again
        }
      } else {
        password += c;
        Serial.print('*'); // Print asterisk for each character
      }
    }
    delay(10);
  }
}

void clear_screen(){
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
}

void print_wifi_menu() {
  // Serial.print("\n\n\n\n\n\n\n");
  Serial.println("\n-------------------WIFI Configuration---------------");
  Serial.println("");


  Serial.print("This is the network store: ");
  Serial.println(getWifiSSID());

  Serial.print("Status of the connection: ");
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("No Internet Connection");
  }
  Serial.println("Connected");

  Serial.print("Strength: ");
  Serial.println(get_rssi_strength_bars());
  
  Serial.println("");
  Serial.println("\n-------------------WIFI Configuration---------------");
  
  Serial.println("");
  Serial.println("");

  Serial.println("1. Select network");
  Serial.println("2. Set password");
  Serial.println("3. Type Credentials manually");
  Serial.println("4. Connect to Wifi");
  Serial.println("q. Quit");
}

void sampling_menu() {
  clear_screen();
  Serial.println("\nSelect the sampling time:");
  Serial.println("1. 10 sec");
  Serial.println("2. 20 sec");
  Serial.println("3. 30 sec");
  Serial.println("4. 1 min");
  Serial.println("5. 5 min");
  Serial.println("6. 10 min");
  Serial.println("7. 1 hour");
  Serial.println("8. Custom");
  Serial.println("9. Or by event"); // TODO
  Serial.println("Or Press any for default 5 min");
}

void print_main_menu() {
  clear_screen();
  Serial.println("\nWelcome to menu select one of the following choices:");
  Serial.println("1. Wifi Configuration");
  Serial.println("2. MODE");
  Serial.println("q. Quit");
}

void print_mode_menu(){
  clear_screen();
  Serial.println("1. MASTER");
  Serial.println("2. SLAVE");
  Serial.println("3. CONTROLLER");
  Serial.println("4. MENU");
  Serial.println("q. Quit");
}

void sampling_handler(int sampling_choice) {
  switch (sampling_choice)
  {
  case 1:
    Serial.print("Your selection was: 1 - 10 sec");
    setSamplingTime(10);
    break;

  case 2:
    Serial.print("Your selection was: 2 - 20 sec");
    setSamplingTime(20);
    break;

  case 3:
    Serial.print("Your selection was: 3 - 30 sec");
    setSamplingTime(30);
    break;

  case 4:
    Serial.print("Your selection was: 4 - 1 min");
    setSamplingTime(60);
    break;

  case 5:
    Serial.print("Your selection was: 5 - 5 min");
    setSamplingTime(60 * 5);
    break;
  
  case 6:
    Serial.print("Your selection was: 6 - 10 min");
    setSamplingTime(60 * 10);
    break;

  case 7:
    Serial.print("Your selection was: 7 - 1 h");
    setSamplingTime(60 * 60);
    break;

  case 8:
    Serial.print("Your selection was: Custom");
    // ToDO
    break;

  case 9:
    Serial.print("Your selection was: By event");
    // ToDO
    break;
  
  default:
    setSamplingTime(60 * 5);
    break;
  }
}
 
void wifi_handler(char wifi_choice, String* ssids) {
  // "1. Select network"
  if(wifi_choice == '1') {
    if (ssids != nullptr) {
      Serial.println("\nWiFi SSIDs found:");
      for (int i = 0; i < sizeof(ssids); ++i) {
        Serial.print(i);
        Serial.print(": ");
        Serial.println(ssids[i]);
      }
      // delete[] ssids;
    }

    while (!Serial.available());
    int wifi_selected = Serial.parseInt();
    Serial.print("Your selection is: ");
    Serial.print(ssids[wifi_selected]);

    Serial.println("Are you sure this is the network that you are looking for? (y/n)");
    while (!Serial.available());
    char answer = Serial.read();

    Serial.print("Your answer is: ");
    Serial.print(answer);
    if(answer == 'y') {
      setWifiSSID(ssids[wifi_selected]);
      Serial.println("Saved!");
    } else {
      Serial.println("No Saved");
    }

    delay(1000);
  }

  // "2. Set password"
  if(wifi_choice == '2') {
    Serial.println("Type the password");
    String pass = getPasswordFromSerial();

    setWifiPassword(pass);

    delay(1000);
    
  }

  if(wifi_choice == '3') {}

  // "4. Connect to Wifi"
  if(wifi_choice == '4') {
    Serial.println("Connectiong with");
    Serial.print("WifiNetwork: ");
    Serial.println(getWifiSSID());

    Serial.print("Pass: ");
    Serial.println(getWifiPassword());

    wifi_connection(getWifiSSID().c_str(), getWifiPassword().c_str());

    while(WiFi.status() != WL_CONNECTED){
      Serial.println("Conecting... ");
      delay(500);
    }
    Serial.println("Connected");
  }

  if(wifi_choice == 'q') {}
}

void mode_handler(char mode_choice) {
  if(mode_choice == '1') {
    setMode("MASTER");
    Serial.println("");
    Serial.println("Is it Seelpy mode? [y/n]");
    while (!Serial.available());
    char answer = Serial.read();

    Serial.print("Your answer is: ");
    Serial.print(answer);
    if(answer == 'y') {
      setIsSleepyMode(true);
      Serial.println("Saved!");
    } else {
      Serial.println("No Saved");
    }
  }

  if(mode_choice == '2') {
    setMode("SLAVE");
  }

  if(mode_choice == '3') {
    setMode("CONTROLLER");
  }

  if(mode_choice == '1' || mode_choice == '2' || mode_choice == '3') {
    sampling_menu();
    while (!Serial.available());
    int sampling_choice = Serial.parseInt();
    sampling_handler(sampling_choice);
  }

  if(mode_choice == '4') {
    setMode("MENU");
  }
}

void main_menu_handler(char main_choise, String* ssids){

  // "1. Wifi Configuration"
  if(main_choise == '1') {
    print_wifi_menu();
    while (!Serial.available());
    char wifi_choice = Serial.read();
    wifi_handler(wifi_choice, ssids);

    // while (!Serial.available());
  }


  // Rest of the code
  // "2. MODE"
  if(main_choise == '2') {
    print_mode_menu();
    while (!Serial.available());
    char mode_choice = Serial.read();
    mode_handler(mode_choice);
  }

  if(main_choise == 'q') {
    Serial.println("");
    Serial.println("You select quit!!");
    done = false;
  }
}

void menu(String* ssids) {
  while(done) {
    print_main_menu();

    while (!Serial.available());
    char main_choise = Serial.read();
    // int main_choise = Serial.parseInt();
    main_menu_handler(main_choise, ssids);
  }
}

