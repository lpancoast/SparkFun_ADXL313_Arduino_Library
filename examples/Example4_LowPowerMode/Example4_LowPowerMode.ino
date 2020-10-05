/******************************************************************************
  Example4_LowPowerMode.ino
  Shows how to use Low Power feature. 
  In addition to turning on low power mode, you will also want to consider
  bandwidth rate. This will affect your results in low power land.
  In this example, we will turn on low power mode and set BW to 12.5Hz.
  Then we will only take samples at or above 12.5Hz (so we don't miss samples)

  SparkFun ADXL313 Arduino Library
  Pete Lewis @ SparkFun Electronics
  Original Creation Date: September 19, 2020
  https://github.com/sparkfun/SparkFun_ADXL313_Arduino_Library

  Do you like this library? Help support SparkFun. Buy a board!

    SparkFun 3-Axis Digital Accelerometer Breakout - ADXL313 (Qwiic)
    https://www.sparkfun.com/products/17241  

  Development environment specifics:

	IDE: Arduino 1.8.13
	Hardware Platform: SparkFun Redboard Qwiic
	SparkFun 3-Axis Digital Accelerometer Breakout - ADXL313 (Qwiic) Version: 1.0

  Hardware Connections:
  Use a qwiic cable to connect from the Redboard Qwiic to the ADXL313 breakout (QWIIC).
  You can also choose to wire up the connections using the header pins like so:

  ARDUINO --> ADXL313
  SDA (A4) --> SDA
  SCL (A5) --> SCL
  GND --> 3.3V
  GND --> GND

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <Wire.h>
#include <SparkFunADXL313.h>
ADXL313 myAdxl;

void setup()
{
  Serial.begin(115200);
  Serial.println("Example 4 - Low power mode ON with 12.5Hz bandwidth.");

  Wire.begin();

  if (myAdxl.begin() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while(1); //Freeze
  }
  Serial.print("Sensor is connected properly.");
  
  myAdxl.standby(); // Must be in standby before changing settings.
                    // This is here just in case we already had sensor powered and/or
                    // configured from a previous setup.

  myAdxl.lowPowerOn();

  myAdxl.setBandwidth(ADXL313_BW_12_5); // 12.5Hz is the best power savings.
  // Other options possible are the following.
  // Note, bandwidths not listed below do not cause power savings.
  // ADXL313_BW_200		    (115uA in low power)
  // ADXL313_BW_100		    (82uA in low power)
  // ADXL313_BW_50		    (64uA in low power)
  // ADXL313_BW_25		    (57uA in low power)
  // ADXL313_BW_12_5	    (50uA in low power)
  // ADXL313_BW_6_25		  (43uA in low power)

  myAdxl.measureModeOn(); // wakes up the sensor from standby and puts it into measurement mode
}

void loop()
{
  updateIntSourceStatuses(); // this will update all class intSource.xxxxx variables by reading int source bits.

  if(myAdxl.intSource.dataReady) // check data ready interrupt bit
  {
    myAdxl.readAccel(); // read all 3 axis, they are stored in class variables: myAdxl.x, myAdxl.y and myAdxl.z
    Serial.print("x: ");
    Serial.print(myAdxl.x);
    Serial.print("\ty: ");
    Serial.print(myAdxl.y);
    Serial.print("\tz: ");
    Serial.print(myAdxl.z);
    Serial.println();
  }
  else
  {
    Serial.println("Waiting for data.");
  }  
  delay(80); // 80ms should put us close to a sample data rate of 12.5 (probably a bit greater due to communication times)
}