#include "MapObject.h"

string MapObject::typeTitle(ObjectType type) {
	switch (type) {
		case LIGHT:
			return "Light";
		case MESH_INSTANCE:
			return "Mesh Instance";
		case PATH_NODE:
			return "Path Node";
		case START_POINT:
			return "Start Point";
		default:
			return "";
	}
}
