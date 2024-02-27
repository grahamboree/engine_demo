require "map"

player_speed = 10
player = create_entity()
set_texture(player, gfx.load_texture("Tiles/grass1.png"))
gfx.add_to_drawlist(player)

-- create_map()

function update(dt)
    -- get the input direction.
    input_dir = { x = 0, y = 0 }
    if io.get_key("up") then
        input_dir.y = 1
    elseif io.get_key("down") then
        input_dir.y = -1
	end
    if io.get_key("left") then
        input_dir.x = -1
    elseif io.get_key("right") then
        input_dir.x = 1
    end
	
    -- move the player sprite.
    pos = get_pos(player)
    pos.x = pos.x + input_dir.x * player_speed * dt
    pos.y = pos.y + input_dir.y * player_speed * dt
    set_pos(player, pos)
end
 