#include "bullet_common.h"

#define DIAL_TUTORIAL_START

BULLET void ConstructDialogs()
{
    RegisterDialogSequence("TUTORIAL_START",
                           "{teacher|excited}Greetings traveler, are you up for the challenge to reach the mountain top?\\\
                            {happy}It won't be an easy journey but I'll try to help you on your way!\\\
                            {neutral}See your spaceship in the middle of the screen? That's you!\\\
                            Move it around with the arrow keys or WASD (will be changed in the future)\\\
                            {shout}Press Z to fire your ammunition! Without it you'll be fried meat before you even realize...");
}
