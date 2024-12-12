#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <regex>
#include <list>


const std::string ENRegex = "^[a-zA-Z0-9!?*~@#$&%;:_|()\'\\-`.+,<>=/\"\\s]+$";
// \xe3\x80\x80 at the end is for the ideographic space
const std::string ENRegex2 = "^[a-zA-Z0-9\\[\\]!?*~@#$&%;:_|()\'\\-`.+,<>=/\"\\s\xe3\x80\x80]+$";
