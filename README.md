# dota2-lua-engine

Lua Scripting Engine for Dota 2

## Usage

Start a match than execute `dota2-esp\tools\dota-inject.bat`

## Commands

```
$ lua_loadscript <file_name>
$ lua_executescript <file_name>
$ lua_unloadscript <file_name>
```

## API

Work in progress.

```
dota.GetLocalPlayer()
dota.GetLocalHero()
dota.FindEntities()
dota.AddPaintHook()
dota.AddEventListener()

dota.PlayerResource
dota.DrawUtils
dota.GlobalInstanceManager
```
