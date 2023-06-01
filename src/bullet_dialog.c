#include "bullet_common.h"

#define DIAL_TUTORIAL_START

BULLET void ConstructDialogs()
{
    RegisterDialogSequence("TUTORIAL_START",
                           "{placeholder|excited}Greetings traveler, are you up for the challenge to reach the mountain top?\\\
                            {placeholder|happy}It won't be an easy journey but I'll try to help you on your way!\\\
                            {placeholder|neutral}See your spaceship in the middle of the screen? That's you!\\\
                            {placeholder|happy}Move it around with the arrow keys or WASD (will be changed in the future)\\\
                            {placeholder|shout}Press Z to fire your ammunition! Without it, you'll be fried meat before you even realize...");
}
