#include <cluttermm.h>

int main(int argc, char** argv)
{
  Clutter::init(&argc, &argv);

  // Get the stage and set its size and color:
  const Glib::RefPtr<Clutter::Stage> stage = Clutter::Stage::get_default();
  stage->set_size(200, 200);
  stage->set_color(Clutter::Color(0, 0, 0, 255)); // black

  Clutter::Color actor_color (0xff, 0xff, 0xff, 0x99);

  // Add a rectangle to the stage:
  const Glib::RefPtr<Clutter::Rectangle> rect = Clutter::Rectangle::create(actor_color);
  rect->set_size(100, 2);
  rect->set_position(20, 20);
  stage->add_actor(rect);
  rect->show();

  // Add a label to the stage:
  const Glib::RefPtr<Clutter::Actor> label =
    Clutter::Text::create("Sans 12", "Some Text", actor_color);
  label->set_size(500, 500);
  label->set_position(20, 150);
  stage->add_actor(label);
  label->show();

  // Show the stage:
  stage->show();

  // Start the main loop, so we can respond to events:
  Clutter::main();

  return 0;
}

