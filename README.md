# SleeperBuild

## Inspiration
Andrew Huberman, an American neuroscientist and podcaster, claimed the importance of the role of light involved in a person's circadian rhythm. He desired a device that could inform a person of the amount of light exposure they've received before sleep and we've answered the call to action.

## What it does
SleeperBuild as a full product is a combined pair of the SleeperBuild Smart Glasses and the SleeperBuild Pod, meant to be worn before the user goes to bed and when the user wakes up in the morning. Activating automatically when worn, the glasses track the amount of light exposure the user receives. When a user's ideal light exposure target is reached, the SleeperBuild Pod, which can fit cleanly in a user's pocket, will let the user know through haptic feedback. The user can interface with the glasses through any device to set their own desired light exposure targets, their own desired bedtime, and see for themselves a plot over the last few days of how much light exposure they've receieved before they sleep and after they wake up; all to help you, build the best sleep possible.

## How we built it
The SleeperBuild Smart Glasses utilize the ergonomic design of everyday sunglasses, adding small augments like a photoresistor to detect light and a touch sensor to detect when the user is wearing the glasses. A single wire is currently required in the design to connect the SleeperBuild Smart Glasses and the SleeperBuild Pod together. The additions to the sunglasses, when worn normally and with the SleeperBuild Pod stored in a user pocket, do not interfere with user comfort and is comparable to Apple's first iPods. Inside the SleeperBuild Pod is an Real-Time Clock (RTC) device to keep accurate track of the time of day, an ESP32 that handles all the processing of the data received from the SleeperBuild Smart Glasses and hosts the webserver for the user to interact with, and an Xbox One Vibration Motor.

## Challenges we ran into
The team initially wanted to host an actual domain website, however with UofT/eduroam's unique certificates required to gain access to the internet made using the ESP32's ability to access the internet a major roadblock in our design. However, after a lot of experimentation and research, we were able to use the ESP32's ability to act as an Access Point to host it's own local webserver which the user could connect to as a public WiFi network.

## Accomplishments that we're proud of
We're proud to have made two firsts in this project:  
1. Overcoming the Wifi challenges on the ESP32 board to host on our LAN network.  
2. Our final result of an erogonomic/comfortable-to-wear design and keeping all our electronics hidden from the user.  
3. Working 22 hours straight through willpower and passion to make this project come to fruition.  
4. Maintaining backup solutions in the event our primary design has a major design oversight (The Team created a simpler bluetooth accessible version of our app and code, in the event that a WiFi solution was not possible)  

## What we learned
Technical Skills:  
Other than diving through documentation surrounding WiFi and bluetooth communication, we had to learn Javascript in the context of Andriod App Development  

Soft Skills:  
- Prioritizing different aspects of the project over others while managing the fundamental design goals became more and more important as the project went on  
- Making best use of your time while waiting on other teammates to finish parts of the project to work on different parts of the project or implement backup solutions was also key to our final result  
- Keeping up team morale and lowering stress with jokes and sharing good food every so often was necessary to making this an enjoyable experience

## What's next for SleeperBuild  
In future designs, we will make the SleeperBuild Smart Glasses and the SleeperBuild Pod completely wireless from each other to maximize user comfort. For users who sleep in bright areas, like the city of Toronto, we would like to track the ambient light while a user sleeps and so propose the SleeprBuild Sleep Mask as the next step in our product line.  
