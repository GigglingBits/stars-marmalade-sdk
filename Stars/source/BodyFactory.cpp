#include "BodyFactory.h"
#include "FactoryManager.h"

#include "Body.h"
#include "CompositeBody.h"
#include "Star.h"
#include "Nugget.h"
#include "Enemy.h"
#include "Buff.h"

#include <sstream>
#include <string>

BodyFactory::BodyFactory() : FactoryBase<BodyTemplate, Body>("root", "bodies", "body") {
}

std::string BodyFactory::PopulateConfig(TiXmlElement* node, BodyTemplate& conf) {
	// read from XML
	char* pc;
	std::string id((pc = (char*)node->Attribute("id")) ? pc : GenerateUniqueId((long)node));
	std::string type((pc = (char*)node->Attribute("type")) ? pc : "");
	std::string shape((pc = (char*)node->Attribute("shape")) ? pc : "");
	std::string fixture((pc = (char*)node->Attribute("fixture")) ? pc : "");
	std::string texture((pc = (char*)node->Attribute("texture")) ? pc : "");

	// add new definitions to other factories
	if (shape.empty()) {
		if (TiXmlElement* subnode = node->FirstChildElement("shape")) {
			shape = FactoryManager::GetShapeFactory().AddConfig(subnode);
		}
	}

	if (fixture.empty()) {
		if (TiXmlElement* subnode = node->FirstChildElement("fixture")) {
			fixture = FactoryManager::GetFixtureFactory().AddConfig(subnode);
		}
	}

	if (texture.empty()) {
		if (TiXmlElement* subnode = node->FirstChildElement("texture")) {
			texture = FactoryManager::GetTextureFactory().AddConfig(subnode);
		}
	}

	// populate the configuration
	conf.Set(id, type, shape, fixture, texture);

	// drag support
	int enabledrag = 0;
	if (node->Attribute("drag", &enabledrag)) {
		conf.SetEnableDrag(enabledrag != 0);
	}

	// revolve support
	int enablerevolve = 0;
	if (node->Attribute("revolve", &enablerevolve)) {
		conf.SetEnableRevolve(enablerevolve != 0);
	}
	
	// collision support
	int enablecollide = 0;
	if (node->Attribute("collide", &enablecollide)) {
		conf.SetEnableCollide(enablecollide != 0);
	}
	
	// check integity
	IwAssertMsg(MYAPP, FactoryManager::GetShapeFactory().ConfigExists(conf.GetShapeId()), ("No shape '%s' could be found. It is referenced by body '%s'.", shape.c_str(), id.c_str()));
	IwAssertMsg(MYAPP, FactoryManager::GetFixtureFactory().ConfigExists(conf.GetFixtureId()), ("No fixture '%s' could be found. It is referenced by body '%s'.", fixture.c_str(), id.c_str()));
	IwAssertMsg(MYAPP, FactoryManager::GetTextureFactory().ConfigExists(conf.GetTextureId()), ("No texture '%s' could be found. It is referenced by body '%s'.", texture.c_str(), id.c_str()));

	// add ports to body
	conf.AddPort("", CIwFVec2(0.0f, 0.0f)); // origin port
	if (TiXmlElement* portnode = node->FirstChildElement("port")) {
		do {
			std::string portid((pc = (char*)portnode->Attribute("id")) ? pc : GenerateUniqueId((long)portnode));
			double x = 0.0f;
			portnode->Attribute("x", &x);
			double y = 0.0f;
			portnode->Attribute("y", &y);

			conf.AddPort(portid, CIwFVec2((float)x, (float)y));
		} while ((portnode = portnode->NextSiblingElement("port")));
	}
	
	// add children to configuration
	if (TiXmlElement* childnode = node->FirstChildElement("child")) {
		do {
			std::string childid((pc = (char*)childnode->Attribute("id")) ? pc : GenerateUniqueId((long)childnode));
			std::string body((pc = (char*)childnode->Attribute("body")) ? pc : "");
			std::string port((pc = (char*)childnode->Attribute("port")) ? pc : "");
			std::string jointtype((pc = (char*)childnode->Attribute("jointtype")) ? pc : "");

			// support inline bodies
			if (body.empty()) {
				if (TiXmlElement* inlinebodynode = childnode->FirstChildElement("body")) {
					body = FactoryManager::GetBodyFactory().AddConfig(inlinebodynode);
				}
			}

			conf.AddChild(childid, body, port, jointtype);

			IwAssertMsg(MYAPP, FactoryManager::GetBodyFactory().ConfigExists(body), ("No body '%s' could be found. It is referenced by body '%s'.", body.c_str(), id.c_str()));

		} while ((childnode = childnode->NextSiblingElement("child")));
	}

	// add more joints
	if (TiXmlElement* childnode = node->FirstChildElement("joint")) {
		do {
			std::string jointid((pc = (char*)childnode->Attribute("id")) ? pc : GenerateUniqueId((long)childnode));
			std::string childa((pc = (char*)childnode->Attribute("childa")) ? pc : "");
			std::string porta((pc = (char*)childnode->Attribute("porta")) ? pc : "");
			std::string childb((pc = (char*)childnode->Attribute("childb")) ? pc : "");
			std::string portb((pc = (char*)childnode->Attribute("portb")) ? pc : "");
			std::string jointtype((pc = (char*)childnode->Attribute("jointtype")) ? pc : "");
			conf.AddJoint(jointid, childa, porta, childb, portb, jointtype);
		} while ((childnode = childnode->NextSiblingElement("joint")));
	}

	return id;
}

Body* BodyFactory::CreateInstance(const BodyTemplate& conf) {
	b2Vec2 position(0.0f, 0.0f);
	float32 angle = 0.0f;

	// create physics configuration
	BodyTemplate copyconf(conf);
	b2BodyDef* bodydef = copyconf.CreatePhysicsBodyDef();
	bodydef->position = position;
	bodydef->angle = angle;

	b2FixtureDef fixturedef(FactoryManager::GetFixtureFactory().GetConfig(copyconf.GetFixtureId()));
	
	ShapeTemplate shapetpl(FactoryManager::GetShapeFactory().GetConfig(copyconf.GetShapeId()));
	b2Shape* shapedef = shapetpl.CreatePhysicsShapeDef();
	
	TextureTemplate texturedef = FactoryManager::GetTextureFactory().GetConfig(copyconf.GetTextureId());

	// compose the physics body and
	// wrap all into a game body class
	fixturedef.shape = shapedef;
	Body* p = NULL;
	std::string instanceid = GenerateInstanceId();
	if (!copyconf.GetType().compare("star")) {
		p = new Star(instanceid, *bodydef, fixturedef, texturedef);
	} else if (!copyconf.GetType().compare("nugget")) {
		p = new Nugget(instanceid, *bodydef, fixturedef, texturedef);
	} else if (!copyconf.GetType().compare("buff")) {
		p = new Buff(instanceid, *bodydef, fixturedef, texturedef);
	} else if (!copyconf.GetType().compare("enemy")) {
		p = new Enemy(instanceid, *bodydef, fixturedef, texturedef);
	} else if (!copyconf.GetChildren().empty()) {
		p = new CompositeBody(instanceid, *bodydef, fixturedef, texturedef);
	} else {
		p = new Body(instanceid, *bodydef, fixturedef, texturedef);
	}

	// additional properties
	p->EnableDragging(copyconf.IsDragEnabled());
	p->EnableRotation(copyconf.IsRevolveEnabled());
	p->EnableCollisions(copyconf.IsCollideEnabled());

	// configure ports if any
	BodyTemplate::PortMap& ports = copyconf.GetPorts();
	for (BodyTemplate::PortMap::iterator it = ports.begin(); it != ports.end(); it++) {
		p->AddPort(it->first, it->second);
	}

	// configure children
	if (!copyconf.GetChildren().empty()) {
		if (CompositeBody* composite = dynamic_cast<CompositeBody*>(p)) {
			BodyTemplate::ChildMap& children = copyconf.GetChildren();
			for (BodyTemplate::ChildMap::iterator it = children.begin(); it != children.end(); it++) {
				composite->AddChild(it->first, it->second);
			}
		} else {
			IwAssertMsg(MYAPP, false, ("Internal factory error. Body '%s' has %i children but isn't of type CompositeBody.", copyconf.GetType().c_str(), (int)copyconf.GetChildren().size()));
		}
	}

	// configure joints
	if (!copyconf.GetJoints().empty()) {
		if (CompositeBody* composite = dynamic_cast<CompositeBody*>(p)) {
			BodyTemplate::JointMap& joints = copyconf.GetJoints();
			for (BodyTemplate::JointMap::iterator it = joints.begin(); it != joints.end(); it++) {
				BodyTemplate::JointElement& joint = it->second;
				composite->AddJoint(joint.JointName, joint.ChildA, joint.PortA, joint.ChildB, joint.PortB, joint.JointType);
			}
		} else {
			IwAssertMsg(MYAPP, false, ("Internal factory error. Body '%s' has %i joints but isn't of type CompositeBody.", copyconf.GetType().c_str(), (int)copyconf.GetJoints().size()));
		}
	}

	// clean up physics configuration
	delete shapedef;
	delete bodydef;

	return p;
}

Body* BodyFactory::CreateWithLocation(const std::string& id, const CIwFVec2& position, float32 angle) {
	Body* p =  FactoryBase<BodyTemplate, Body>::Create(id);
	p->SetPosition(position, angle);
	//p->GetBody().SetTransform(b2Vec2(position.x, position.y), angle);
	return p;
}
