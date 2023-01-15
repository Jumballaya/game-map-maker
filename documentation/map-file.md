# XML Based TileMap file

## Elements

### XML Header
Standard version = 1.0, encoding is UTF-8

```xml
<?xml version="1.0" encoding="UTF-8" ?>
```

### Map Element

- Name: `map`
- Attributes:
    - `width` - Width of the tile map in tiles
    - `height` - Height of the tile map in tiles
    - `tilesize` - Pixel size of the tile. Single integer value representing the length of each side.
- Children:
    - `tilesets` - List of tilesets
    - `layers` - List of layers

### Tilesets and Tileset Elements

- Name: `tilesets`
- Children:
    - `tileset` - List of sibling tilesets

- Name: `tileset`
- Attributes:
    - `name` - Name of the tileset. A user friendly tag
    - `source` - Filepath of the tileset image, relative to the project root
    - `tileSize` - Same as the tileSize above, but applies only to this tileset
    - `columns` - Number of columns of tiles
    - `rows` - Number of rows of tiles
    - `width` - Width of image in pixels
    - `height` - Height of image in pixels

### Layers and Layer Elements

- Name: `layers`
- Children:
    - `layer` - List of sibling layers

- Name: `layer`
- Children:
    - `tiledata` - Single child with tiledata
- Attributes:
    - `name` - User given name of the layer
    - `width` - Width of the layer in tiles
    - `height` - Width of the layer in tiles
    - `tileset` - Name of the tileset to use

### Tiledata Element

- Name: `tiledata`
- Children:
    - Single text node child with CSV data

The CSV data is a list of indices for tiles from the layer's tileset.

The indices are created from left -> right, top -> bottom.