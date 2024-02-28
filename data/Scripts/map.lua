textures = {
    gfx.load_texture("Tiles/tile_0000.png"),
    gfx.load_texture("Tiles/tile_0001.png"),
    gfx.load_texture("Tiles/tile_0002.png")
}

width = 30;
height = 30;

map_entities = {}

function create_map()
    log("creating tile map")
    -- Create map tiles
    for x = 1,width do
        map_entities[x] = {}
        for y = 1,height do
            tile = create_entity()
            map_entities[x][y] = create_entity()
            set_texture(tile, textures[math.random(#textures)])
            set_pos(tile, {x = x - 0.5 * width, y = y - 0.5 * height})
            gfx.add_to_drawlist(tile)
            map_entities[x][y] = tile
        end
    end
end