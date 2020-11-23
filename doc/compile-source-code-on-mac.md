# Compile Source Code on macOS

![](https://i.imgur.com/4vuxWPr.jpg)

Right click on the `climate-change` folder and click `Get Info`.

![](https://i.imgur.com/6WSkmV7.png)

Copy the location of the folder.

![](https://i.imgur.com/2OIyLeS.png)

Open `Terminal` and type

```bash
cd <the-path-you-just-copied>
```

Be careful if there are spaces in any folder along the path, if this is the case, add a `\` in front of each space.

For example, `Users/Jerry/Documents/CSE 201` should become `Users/Jerry/Documents/CSE\ 201`

Then type the following commands

```bash
cd climate-change
cd src
```

Then type `ls` to check if `SConstruct` is in this folder, if so, the next step is

```bash
scons platform=osx
```

If nothing goes wrong, the dynamic library is generated successfully.

