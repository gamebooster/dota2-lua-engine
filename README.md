# dota2-lua-engine

Lua Scripting Engine for Dota 2

## Usage

Start a match than execute `dota2-esp\tools\dota-inject.bat`

## Commands

```
$ lua_execute <file_name>
$ lua_unload <file_name>
```

## API

Work in progress.

```
dota.GetLocalPlayer()
dota.GetLocalHero()
dota.FindEntities()
dota.AddPaintHook()
dota.AddEventListener()
dota.AddConsoleCommand()

dota.PlayerResource
dota.DrawUtils
dota.GlobalInstanceManager
```

## Examples

You can find multiple example-scripts in `scripts`.
