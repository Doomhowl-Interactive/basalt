import rand
import time
import gx
import gg

const (
    bg_color = gx.rgb(0,50,100)
    grid_color = gx.rgb(200,200,200)
    highlight_color = gx.rgb(230,200,20)
    grid_thick = 2 
)

struct Camera {
mut:
    x int 
    y int 
}

struct Editor {
mut:
    gg &gg.Context = unsafe { nil }
    cam Camera

    grid_size int = 16

    window_width int
    window_height int

    mouse_cell_x int
    mouse_cell_y int
}

fn draw_grid(ed Editor, color gx.Color, thick int, mut ctx gg.Context) {
    start_x := ed.cam.x % ed.grid_size
    start_y := ed.cam.y % ed.grid_size

    end_x := ed.window_width - ed.cam.x % ed.grid_size 
    end_y := ed.window_height - ed.cam.y % ed.grid_size 

    assert(start_x < end_x)
    assert(start_y < end_y)

    assert(ed.grid_size > 0)

    mut x := start_x
    mut y := start_y
    for y < end_y {
        for x < end_x {
            ctx.draw_circle_filled(x, y, thick, color)
            x += ed.grid_size
        }
        y += ed.grid_size
        x = start_x
    }
}

fn on_frame(mut ed Editor){
    mut ctx := ed.gg 

    ctx.begin()

    // draw background tile grid
    draw_grid(ed, grid_color, grid_thick, mut ctx)

    // highlight selected grid
    cur_cell_x := ed.mouse_cell_x * ed.grid_size
    cur_cell_y := ed.mouse_cell_y * ed.grid_size
    ctx.draw_rect_empty(cur_cell_x, cur_cell_y, ed.grid_size,
                    ed.grid_size, highlight_color)

    ctx.end()
}

fn on_event(e &gg.Event, mut ed Editor) {
    // apply resized window dimensions
    ed.window_width = e.window_width
    ed.window_height = e.window_height

    // set selected cell pos
    ed.mouse_cell_x = int(e.mouse_x / ed.grid_size)
    ed.mouse_cell_y = int(e.mouse_y / ed.grid_size)

    // check for key input
    scroll_speed := 12 
    if e.typ == .key_down {
        match e.key_code {
            .d  {
                ed.cam.x += scroll_speed
            }
            .a  {
                ed.cam.x -= scroll_speed
            }
            .w  {
                ed.cam.y -= scroll_speed
            }
            .s  {
                ed.cam.y += scroll_speed
            }

            .page_up {
                if ed.grid_size < 256 {
                    ed.grid_size *= 2
                    println("Resized grid to ${ed.grid_size}")
                }
            }
            .page_down {
                if ed.grid_size > 4 {
                    ed.grid_size /= 2
                    println("Resized grid to ${ed.grid_size}")
                }
            }

            else {}
        }
    }
}

fn main(){

    width := 640
    height := 480

    mut editor := &Editor{
        gg: 0
        window_width: width
        window_height: height
    }

    editor.gg = gg.new_context(
        bg_color: bg_color
        width: width
        height: height
        create_window: true
        window_title: 'Basalt Map Editor'
        user_data: editor
        frame_fn: on_frame
        event_fn: on_event
    )
    editor.gg.run()
}
