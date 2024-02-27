textures = {
    gfx.load_texture("Tiles/tile_0000.png"),
    gfx.load_texture("Tiles/tile_0001.png"),
    gfx.load_texture("Tiles/tile_0002.png"),
    gfx.load_texture("Tiles/tile_0003.png"),
    gfx.load_texture("Tiles/tile_0004.png"),
    -- gfx.load_texture("Tiles/tile0000.png"),
}

width = 10;
height = 10;

map_entities = {
}

function create_map()
    -- Create map tiles
    for x = 1,width do
        map_entities[x] = {}
        for y = 1,height do
            map_entities[x][y] = create_entity()
        end
    end
end