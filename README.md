# Diplomacy
This website allows you to play Diplomacy online!

The core engine is written in C++, and server code is written in javascript with Node, and Socket.io

Must install nlohmann/json. Use Homebrew and type <strong>brew tap nlohmann/json</strong> and then <strong>brew install nlohmann_json</strong>.
To prepare the adjudicator to run as a child process, install <strong>node-gyp</strong> globally from npm, and run <strong>node-gyp generate build</strong> in the Adjudicator directory.
