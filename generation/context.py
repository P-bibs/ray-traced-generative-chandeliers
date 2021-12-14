

context = {
    "lights": []
}

next_id = 0

def get_id():
    global next_id
    next_id += 1
    return next_id
    

def get_lights():
    return context['lights']

def append_light(light):
    context['lights'].append(light)

