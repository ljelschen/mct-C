
### Programming Buttton

   Install the Addon to VS-Code
   https://marketplace.visualstudio.com/items?itemName=seunlanlege.action-buttons

   Add this to VS-Code for the "Build and Flash" Button
   
   ```
    "actionButtons": {
        "defaultColor": "#ff0034", // Can also use string color names.
        "loadNpmCommands":false, // Disables automatic generation of actions for npm commands.
        "reloadButton":"♻️", // Custom reload button text or icon (default ↻). null value enables automatic reload on configuration change
        "commands": [
            {
                "name": "▶️Build and Flash💻C",
                "color": "green",
                "command": ".\\flash.bat '${file}'",
            }
        ]
    }
```