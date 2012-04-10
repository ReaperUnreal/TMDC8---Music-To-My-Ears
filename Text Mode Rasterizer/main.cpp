#include "main.h"

Console *console = NULL;
Graphics *graphics = NULL;
Rasterizer *rasterizer = NULL;
Font *font = NULL;
FSOUND_STREAM *song = NULL;
ResourceManager *rm = NULL;
int shape = 1;

void init_resources(void)
{
	Mesh *mesh = new Mesh();
	mesh->ReadFrom("data/note.gmf");
	mesh->ReColour(Color(1, 1, 0));
	mesh->RotateAboutX(PI / 2.0f);
	mesh->Translate(-1.7f, 1.7f, 1.8f);
	rm->AddMesh(mesh);
	mesh = new Mesh();
	mesh->ReadFrom("data/note.gmf");
	mesh->ReColour(Color(1, 1, 0));
	mesh->RotateAboutX(PI / 2.0f);
	mesh->MoveTo(-0.1f, 1.7f, 1.8f);
	rm->AddMesh(mesh);
	mesh = new Mesh();
	mesh->ReadFrom("data/note.gmf");
	mesh->ReColour(Color(1, 1, 0));
	mesh->RotateAboutX(PI / 2.0f);
	mesh->Translate(0.4f, 1.7f, 1.8f);
	rm->AddMesh(mesh);
	mesh = new Mesh();
	mesh->ReadFrom("data/trumpet.gmf");
	mesh->ReColour(Color(1, 1, 0));
	mesh->Translate(4.0f);
	rm->AddMesh(mesh);
	mesh = new Mesh();
	mesh->ReadFrom("data/trumpet.gmf");
	mesh->ReColour(Color(1, 1, 0));
	mesh->RotateAboutY(PI);
	mesh->Translate(4.0f);
	rm->AddMesh(mesh);
	mesh = new Mesh();
	mesh->ReadFrom("data/ear.gmf");
	mesh->ReColour(Color(1, 1, 1));
	mesh->RotateAboutY(PIBY2 + PIBY3);
	mesh->Translate(-0.7f, -0.1f, 1.5f);
	rm->AddMesh(mesh);
	mesh = new Mesh();
	mesh->ReadFrom("data/ball.gmf");
	mesh->ReColour(Color(1, 1, 1));
	mesh->Translate(0.0f, 0.0f, 3.5f);
	mesh->ScaleX(0.05f);
	mesh->ScaleY(0.05f);
	mesh->ScaleZ(0.05f);
	rm->AddMesh(mesh);


	float *f = new float;
	*f = 0;
	rm->AddFloat(f);
	f = new float;
	*f = 0;
	rm->AddFloat(f);
	f = new float;
	*f = 0;
	rm->AddFloat(f);
	f = new float;
	*f = 0;
	rm->AddFloat(f);
	f = new float;
	*f = 1;
	rm->AddFloat(f);
	f = new float;
	*f = 0;
	rm->AddFloat(f);

	Tunnel *tu = new Tunnel(graphics->GetColorMap(), Color(), Color());
	rm->AddBackFill(tu);
	CircleFill *cf = new CircleFill(0.0f, Color());
	rm->AddBackFill(cf);
	RadialFill *rf = new RadialFill(Color(), Color(1, 0, 0), 0);
	rm->AddBackFill(rf);
	cf = new CircleFill(0.0f, Color(1, 0.5f, 0.5f));
	rm->AddBackFill(cf);
	Metaballs *mb = new Metaballs(Color(1, 0.5f, 0.5f), Color(1, 0.5f, 0.5f));
	rm->AddBackFill(mb);
	Fire *fire = new Fire(graphics->GetColorMap());
	rm->AddBackFill(fire);
}

void clear_resources(void)
{
	rm->DeleteAll();
}

void text_intro(float val, int lnum)
{
	graphics->Clear();
	if(lnum < 5)
	{
		//font->PrintCharacter('T', Color(1, 1, 1), 30, 19);
		font->PrintPhrase("Music", Color(1, 1, 1), 15, 5);
		if(lnum >= 1)
		{
			//font->PrintCharacter('M', Color(1, 1, 1), 34, 19);
			font->PrintPhrase("To", Color(1, 1, 1), 34, 11);
			if(lnum >= 2)
			{
				//font->PrintCharacter('D', Color(1, 1, 1), 40, 19);
				font->PrintPhrase("My", Color(1, 1, 1), 38, 17);
				if(lnum >= 3)
				{
					//font->PrintCharacter('C', Color(1, 1, 1), 45, 19);
					font->PrintPhrase("Ears", Color(1, 1, 1), 45, 23);
					if(lnum == 4)
					{
						font->PrintPhrase("TMDC", Color(val, val, val), 26, 34);
						font->PrintCharacter('8', Color(0.5f * val, 0, 0), 50, 32);
						font->PrintCharacter('8', Color(0, 0.5f * val, 0), 52, 36);
						font->PrintCharacter('8', Color(0, 0, 0.5f * val), 48, 36);
						font->PrintCharacter('8', Color(val, val, val), 50, 34);
					}
				}
			}
		}
	}
	else
	{
		RadialFill rf(Color(0, 0, 0), Color(1, 1, 1), (val - 0.5f) * 100.0f);
		font->PrintPhrase("Music", Color(1, 1, 1), 15, 5);
		font->PrintPhrase("To", Color(1, 1, 1), 34, 11);
		font->PrintPhrase("My", Color(1, 1, 1), 38, 17);
		font->PrintPhrase("Ears", Color(1, 1, 1), 45, 23);
		font->PrintPhrase("TMDC", Color(val, val, val), 26, 34);
		font->PrintCharacter('8', Color(0.5f * val, 0, 0), 50, 32);
		font->PrintCharacter('8', Color(0, 0.5f * val, 0), 52, 36);
		font->PrintCharacter('8', Color(0, 0, 0.5f * val), 48, 36);
		font->PrintCharacter('8', Color(val, val, val), 50, 34);
		graphics->FillBuffer(&rf);
	}
	graphics->Present();
}

void fade_from_white(float val)
{
	ColorFill cf(Color(val, val, val));
	graphics->Clear();
	graphics->FillBuffer(&cf);
	graphics->Present();
}

void black_pause(void)
{
	graphics->Clear();
	graphics->Present();
}

void bounce_notes(int stepNum, float val)
{
	Mesh *note1, *note2, *note3;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	switch(stepNum)
	{
	case 0:
		note1->MoveTo(-1.7f + 1.1f * val, 1.7f - 1.7f * val * val, 1.8f);
		break;
	case 1:
		note1->MoveTo(-0.6f, val * (2 - 2 * val), 1.8f);
		break;
	case 2:
		note1->MoveTo(-0.6f, val * (2 - 2 * val), 1.8f);
		note2->MoveTo(-0.1f, 1.7f - 1.7f * val * val, 1.8f);
		break;
	case 3:
		note1->MoveTo(-0.6f, val * (2 - 2 * val), 1.8f);
		note2->MoveTo(-0.1f, val * (2 - 2 * val), 1.8f);
		break;
	case 4:
		note1->MoveTo(-0.6f, val * (2 - 2 * val), 1.8f);
		note2->MoveTo(-0.1f, val * (2 - 2 * val), 1.8f);
		note3->MoveTo(1.5f - 1.1f * val, 1.7f - 1.7f * val * val, 1.8f);
		break;
	case 5:
		note1->MoveTo(-0.6f, val * (2 - 2 * val), 1.8f);
		note2->MoveTo(-0.1f, val * (2 - 2 * val), 1.8f);
		note3->MoveTo(0.4f, val * (2 - 2 * val), 1.8f);
		break;
	case 6:
		if(val > 0.5f)
			val = 0.5f;
		note1->MoveTo(-0.6f, val * (2 - 2 * val), 1.8f);
		note2->MoveTo(-0.1f, val * (2 - 2 * val), 1.8f);
		note3->MoveTo(0.4f, val * (2 - 2 * val), 1.8f);
		break;
	default:
		break;
	}
	rasterizer->AddToRenderList(note1);
	rasterizer->AddToRenderList(note2);
	rasterizer->AddToRenderList(note3);

	graphics->Clear();
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void twirl_notes(float val)
{
	Mesh *note1, *note2, *note3;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	float *prevval = rm->GetFloat(0);
	float diff = val - *prevval;
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	if(val <= 1.0f)
	{
		note1->RotateAboutY(diff * FOURPI);
	}
	else if(*prevval < 1.0f)
	{
		note1->RotateAboutY((1 - *prevval) * FOURPI);
	}

	if(val > 0.5f)
	{
		if(val < 1.5f)
		{
			if(*prevval < 0.5f)
			{
				note2->RotateAboutY((val - 0.5f) * FOURPI);
			}
			else
			{
				note2->RotateAboutY(diff * FOURPI);
			}
		}
		else if(*prevval < 1.5f)
		{
			note2->RotateAboutY((1.5f - *prevval) * FOURPI);
		}
	}

	if(val > 1.0f)
	{
		if(val < 2.0f)
		{
			if(*prevval < 1.0f)
			{
				note3->RotateAboutY((val - 1.0f) * FOURPI);
			}
			else
			{
				note3->RotateAboutY(diff * FOURPI);
			}
		}
		else if(*prevval < 2.0f)
		{
			note3->RotateAboutY((2.0f - *prevval) * FOURPI);
		}
	}

	rasterizer->AddToRenderList(note1);
	rasterizer->AddToRenderList(note2);
	rasterizer->AddToRenderList(note3);

	graphics->Clear();
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();

	*prevval = val;
}

void fly_away_notes(float val)
{
	Mesh *note1, *note2, *note3;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	note1->MoveTo(-0.6f - val, 0.5f, 1.8f + 58.2f * val * val);
	if(val > 0.25f)
	{
		val -= 0.25f;
		note2->MoveTo(-0.1f - val, 0.5f, 1.8f + 58.2f * val * val);
		if(val > 0.25f)
		{
			val -= 0.25f;
			note3->MoveTo(0.4f - val, 0.5f, 1.8f + 58.2f * val * val);
		}
	}

	rasterizer->AddToRenderList(note1);
	rasterizer->AddToRenderList(note2);
	rasterizer->AddToRenderList(note3);

	graphics->Clear();
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void zoom_to_notes(float val)
{
	Mesh *note1, *note2, *note3;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	float sval = (1 - cosf(val * PI)) * 0.5f;
	note1->MoveTo(1.6f * val - 1.6f, 0.5f - 0.4f * val, 60.0f - 57.9f * sval);
	note2->MoveTo(1.05f * val - 0.85f, 0.5f - 0.4f * val, 34.5375f - 32.7375f * sval);
	note3->MoveTo(0.5f * val - 0.1f, 0.5f - 0.4f * val, 16.35f - 14.85f * sval);

	rasterizer->AddToRenderList(note1);
	rasterizer->AddToRenderList(note2);
	rasterizer->AddToRenderList(note3);

	graphics->Clear();
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void prepare_for_swirl(float val)
{
	Mesh *note1, *note2, *note3;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	note1->MoveTo(0, lerp(0.1f, 1.0f, val), lerp(2.1f, 2.4f, val));
	note2->MoveTo(lerp(0.2f, -ROOT3BY2, val), lerp(0.1f, -0.5f, val), lerp(1.8f, 2.4f, val));
	note3->MoveTo(lerp(0.4f, ROOT3BY2, val), lerp(0.1f, -0.5f, val), lerp(1.5f, 2.4f, val));

	rasterizer->AddToRenderList(note1);
	rasterizer->AddToRenderList(note2);
	rasterizer->AddToRenderList(note3);

	graphics->Clear();
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void swirl_note(float val)
{
	Mesh *note1, *note2, *note3;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	val *= -PI;
	note1->MoveTo(cosf(val + PIBY2), sinf(val + PIBY2), 2.4f);
	note2->MoveTo(cosf(val + 7.0f * PIBY6), sinf(val + 7.0f * PIBY6), 2.4f);
	note3->MoveTo(cosf(val - PIBY6), sinf(val - PIBY6), 2.4f);

	rasterizer->AddToRenderList(note1);
	rasterizer->AddToRenderList(note2);
	rasterizer->AddToRenderList(note3);

	graphics->Clear();
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void fade_in_tunnel(float val)
{
	Mesh *note1, *note2, *note3;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	Tunnel *tu = (Tunnel *)rm->GetBackFill(0);
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	note1->MoveTo(0.0f, lerp(-1.0f, 1.0f, val), 2.4f);
	note2->MoveTo(lerp(ROOT3BY2, -ROOT3BY2, val), lerp(0.5f, -0.5f, val), 2.4f);
	note3->MoveTo(lerp(-ROOT3BY2, ROOT3BY2, val), lerp(0.5f, -0.5f, val), 2.4f);

	rasterizer->AddToRenderList(note1);
	rasterizer->AddToRenderList(note2);
	rasterizer->AddToRenderList(note3);
	tu->SetColors(graphics->GetColorMap(), Color(val, val, val), Color());

	graphics->Clear();
	graphics->FillBuffer(tu);
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void move_tunnel(float val)
{
	Mesh *note1, *note2, *note3;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	Tunnel *tu = (Tunnel *)rm->GetBackFill(0);
	float *prevval = rm->GetFloat(1);
	float diff = val - *prevval;
	Light light;
	float theta1 = val * TWOPI + PIBY2;
	float theta2 = val * TWOPI + 7.0f * PIBY6;
	float theta3 = val * TWOPI - PIBY6;
	float theta4 = 0.0f;

	if(val < 2.0f)
		theta4 = diff * FOURPI;
	else if(*prevval < 2.0f)
		theta4 = (2.0f - *prevval) * FOURPI;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	tu->SetPhase(val * 5.0f);
	tu->SetUPhase(-val);
	tu->SetVPhase(val * 4.0f);

	float r = 0.75f + 0.25f * cosf(3.0f * theta1);
	note1->MoveTo(r * cosf(theta1), r * sinf(theta1), 2.4f);
	note1->RotateAboutY(theta4);
	r = 0.75f + 0.25f * cosf(3.0f * theta2);
	note2->MoveTo(r * cosf(theta2), r * sinf(theta2), 2.4f);
	note2->RotateAboutY(theta4);
	r = 0.75f + 0.25f * cosf(3.0f * theta3);
	note3->MoveTo(r * cosf(theta3), r * sinf(theta3), 2.4f);
	note3->RotateAboutY(theta4);

	rasterizer->AddToRenderList(note1);
	rasterizer->AddToRenderList(note2);
	rasterizer->AddToRenderList(note3);

	graphics->Clear();
	graphics->FillBuffer(tu);
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();

	*prevval = val;
}

void tunnel_pulse(float val)
{
	Mesh *note1, *note2, *note3;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	Tunnel *tu = (Tunnel *)rm->GetBackFill(0);
	Light light;
	float theta1 = val * TWOPI + PIBY2;
	float theta2 = val * TWOPI + 7.0f * PIBY6;
	float theta3 = val * TWOPI - PIBY6;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	tu->SetPhase(val * 5.0f + 10.0f);
	tu->SetUPhase(-val - 2.0f);
	tu->SetVPhase(val * 4.0f + 8.0f);

	float invzinc = fabsf(sinf((val - 0.0625f) * EIGHTPI));
	float zinc = 1.0f - invzinc;

	float r = 0.75f + 0.25f * cosf(3.0f * theta1);
	note1->ReColour(Color(1.0f, invzinc, 0.0f));
	note1->MoveTo(r * cosf(theta1), r * sinf(theta1), 2.4f - zinc);
	r = 0.75f + 0.25f * cosf(3.0f * theta2);
	note2->ReColour(Color(invzinc, 1.0f, 0.0f));
	note2->MoveTo(r * cosf(theta2), r * sinf(theta2), 2.4f - zinc);
	r = 0.75f + 0.25f * cosf(3.0f * theta3);
	note3->ReColour(Color(invzinc, invzinc, zinc));
	note3->MoveTo(r * cosf(theta3), r * sinf(theta3), 2.4f - zinc);

	rasterizer->AddToRenderList(note1);
	rasterizer->AddToRenderList(note2);
	rasterizer->AddToRenderList(note3);

	graphics->Clear();
	graphics->FillBuffer(tu);
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void tunnel_circle_forward(float val)
{
	Mesh *note1, *note2, *note3;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	Tunnel *tu = (Tunnel *)rm->GetBackFill(0);
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	tu->SetPhase(val * 5.0f + 15.0f);
	tu->SetUPhase(-val - 3.0f);
	tu->SetVPhase(val * 4.0f + 12.0f);

	val *= TWOPI;
	float ct = cosf(val), st = sinf(val) * 1.2f + 2.4f;
	note1->MoveTo(ct * cosf(val + PIBY2), ct * sinf(val + PIBY2), st);
	note2->MoveTo(ct * cosf(val + 7.0f * PIBY6), ct * sinf(val + 7.0f * PIBY6), st);
	note3->MoveTo(ct * cosf(val - PIBY6), ct * sinf(val - PIBY6), st);

	rasterizer->AddToRenderList(note1);
	rasterizer->AddToRenderList(note2);
	rasterizer->AddToRenderList(note3);

	graphics->Clear();
	graphics->FillBuffer(tu);
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void tunnel_fade_gold(float val)
{
	Mesh *note1, *note2, *note3;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	Tunnel *tu = (Tunnel *)rm->GetBackFill(0);
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	tu->SetPhase(val * 5.0f + 15.0f);
	tu->SetUPhase(-val - 3.0f);
	tu->SetVPhase(val * 4.0f + 12.0f);
	tu->SetColors(graphics->GetColorMap(), Color(1.0f, 1.0f, 1.0f - val), Color(val * 0.2f, val * 0.2f, 0.0f));

	float r = 1.0f - val;
	float theta = val * FOURPI;

	note1->MoveTo(r * cosf(theta + PIBY2), r * sinf(theta + PIBY2) + 0.1f * val, lerp(2.4f, 2.1f, val));
	note1->ReColour(Color(1.0f, 1.0f - val, 0));
	note2->MoveTo(r * cosf(theta + 7.0f * PIBY6) + 0.2f * val, r * sinf(theta + 7.0f * PIBY6) + 0.1f * val, lerp(2.4f, 1.8f, val));
	note2->ReColour(Color(1.0f - val, 1.0f, 0));
	note3->MoveTo(r * cosf(theta - PIBY6) + 0.4f * val, r * sinf(theta - PIBY6) + 0.1f * val, lerp(2.4f, 1.5f, val));
	note3->ReColour(Color(1.0f - val, 1.0f - val, val));

	rasterizer->AddToRenderList(note1);
	rasterizer->AddToRenderList(note2);
	rasterizer->AddToRenderList(note3);

	graphics->Clear();
	graphics->FillBuffer(tu);
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void move_notes_golden_tunnel(float val)
{
	Mesh *note1, *note2, *note3;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	Tunnel *tu = (Tunnel *)rm->GetBackFill(0);
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	tu->SetPhase(val * 5.0f + 20.0f);
	tu->SetUPhase(-val - 4.0f);
	tu->SetVPhase(val * 4.0f + 16.0f);

	float amplitude = (1.0f - cosf(val * TWOPI)) * 0.1f;

	note1->MoveTo(0, amplitude * sinf(SIXTEENPI * val) + 0.1f, 2.1f);
	note2->MoveTo(0.2f, amplitude * sinf(SIXTEENPI * val + PIBY2) + 0.1f, 1.8f);
	note3->MoveTo(0.4f, amplitude * sinf(SIXTEENPI * val + PI) + 0.1f, 1.5f);

	rasterizer->AddToRenderList(note1);
	rasterizer->AddToRenderList(note2);
	rasterizer->AddToRenderList(note3);

	graphics->Clear();
	graphics->FillBuffer(tu);
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void pause_tunnel(float val)
{
	Mesh *note1, *note2, *note3;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	Tunnel *tu = (Tunnel *)rm->GetBackFill(0);
	CircleFill *cf = (CircleFill *)rm->GetBackFill(1);
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	tu->SetPhase(val * 2.5f + 25.0f);
	tu->SetUPhase(-0.5f * val - 5.0f);
	tu->SetVPhase(val * 2.0f + 20.0f);
	cf->SetRadius(val * val * val * 95.0f);

	rasterizer->AddToRenderList(note1);
	rasterizer->AddToRenderList(note2);
	rasterizer->AddToRenderList(note3);

	graphics->Clear();
	graphics->FillBuffer(tu);
	graphics->FillBuffer(cf);
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void fade_out_notes(float val)
{
	Mesh *note1, *note2, *note3;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	float cval = MAX(1.0f - 1.5f * val, 0.0f);

	note1->ReColour(Color(cval, 0, 0));
	note2->ReColour(Color(0, cval, 0));
	note3->ReColour(Color(0, 0, cval));

	rasterizer->AddToRenderList(note1);
	rasterizer->AddToRenderList(note2);
	rasterizer->AddToRenderList(note3);

	graphics->Clear();
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void show_instrument(float val, int shotnum)
{
	Mesh *trumpet1 = rm->GetMesh(3);
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);
	float cval;

	switch(shotnum)
	{
	case 0:
		cval = MIN(sinf(PI * val) * 1.5f, 1.0f);
		trumpet1->MoveTo(slerp(0.3f, -0.3f, val), lerp(-0.55f, -0.45f, val), 1.9f);
		trumpet1->ReColour(Color(cval, cval, 0));
		break;
	case 1:
		cval = MIN(sinf(PI * val) * 1.5f, 1.0f);
		trumpet1->MoveTo(lerp(-0.2f, 0.4f, val), slerp(0.5f, 0.1f, val), 1.9f);
		trumpet1->ReColour(Color(cval, cval, 0));
		break;
	case 2:
		cval = MIN(sinf(PI * val) * 1.5f, 1.0f);
		trumpet1->MoveTo(slerp(1.7f, 2.0f, val), lerp(0.1f, -0.5f, val), 1.7f);
		trumpet1->ReColour(Color(cval, cval, 0));
		break;
	case 3:
		cval = MIN(sinf(PI * val) * 1.5f, 1.0f);
		trumpet1->MoveTo(lerp(-0.75f, -2.0f, val), slerp(0.5f, -0.5f, val), 2.1f);
		trumpet1->ReColour(Color(cval, cval, 0));
		break;
	case 4:
		cval = MIN(2.0f * val, 1.0f);
		trumpet1->MoveTo(slerp(0.7f, -1.8f, val * val * val * val), slerp(0.0f, -1.0f, val * val * val), slerp(1.7f, 3.5f, val));
		trumpet1->ReColour(Color(cval, cval, 0));
	default: break;
	};

	rasterizer->AddToRenderList(trumpet1);

	graphics->Clear();
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void recolour_notes(void)
{
	Mesh *note1, *note2, *note3;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	note1->ReColour(Color(1, 0, 0));
	note2->ReColour(Color(0, 1, 0));
	note3->ReColour(Color(0, 0, 1));
}

void notes_fly_out(float val)
{
	Mesh *note1, *note2, *note3, *trumpet;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	trumpet = rm->GetMesh(3);
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);
	float val1 = MIN(val * 2.0f, 1.0f);
	float val2 = MIN((val - 0.25f) * 2.0f, 1.0f);
	float val3 = MIN((val - 0.5f) * 2.0f, 1.0f);

	note1->MoveTo(-6.2f * val1 * val1 + 5.3f * val1 - 0.1f, lerp(-0.7f, 0.5f, val1), lerp(3.3f, 1.0f, val1));
	rasterizer->AddToRenderList(note1);

	if(val2 > 0.0f)
	{
		note2->MoveTo(-6.2f * val2 * val2 + 5.3f * val2 - 0.1f, lerp(-0.7f, 0.5f, val2), lerp(3.3f, 1.0f, val2));
		rasterizer->AddToRenderList(note2);
	}
	if(val3 > 0.0f)
	{
		note3->MoveTo(-6.2f * val3 * val3 + 5.3f * val3 - 0.1f, lerp(-0.7f, 0.5f, val3), lerp(3.3f, 1.0f, val3));
		rasterizer->AddToRenderList(note3);
	}

	rasterizer->AddToRenderList(trumpet);

	graphics->Clear();
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void notes_circle_across_screen(float val)
{
	Mesh *note1, *note2, *note3;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	RadialFill *rf = (RadialFill *)rm->GetBackFill(2);
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);
	float val1 = MIN(val * 1.25f, 1.0f);
	float val2 = MIN((val - 0.1f) * 1.25f, 1.0f);
	float val3 = MIN((val - 0.2f) * 1.25f, 1.0f);

	rf->SetRadius((1.0f - fabsf(cosf(FOURPI * val))) * 55.0f - 51.0f);

	if(val < 0.25f)
	{
		rf->SetInnerColor(Color(1, 1, 1));
	}
	else if(val < 0.5f)
	{
		rf->SetInnerColor(Color(1, 0, 0));
	}
	else if(val < 0.75f)
	{
		rf->SetInnerColor(Color(0, 1, 0));
	}
	else
	{
		rf->SetInnerColor(Color(0, 0, 1));
	}

	if(val1 <= 0.25f)
	{
		note1->MoveTo(lerp(2.3f, 0.0f, 4.0f * val1), -1.1f, 3.0f);
		rasterizer->AddToRenderList(note1);
	}
	else if(val1 <= 0.75f)
	{
		val1 = lerp(PIBY2 + PI, -PIBY2, 2.0f * (val1 - 0.25f));
		note1->MoveTo(1.1f * cosf(val1), 1.1f * sinf(val1), 3.0f);
		rasterizer->AddToRenderList(note1);
	}
	else if(val1 <= 1.0f)
	{
		note1->MoveTo(lerp(0.0f, -2.3f, 4.0f * (val1 - 0.75f)), -1.1f, 3.0f);
		rasterizer->AddToRenderList(note1);
	}

	if(val2 > 0.0f)
	{
		if(val2 <= 0.25f)
		{
			note2->MoveTo(lerp(2.3f, 0.0f, 4.0f * val2), -1.1f, 3.0f);
			rasterizer->AddToRenderList(note2);
		}
		else if(val2 <= 0.75f)
		{
			val2 = lerp(PIBY2 + PI, -PIBY2, 2.0f * (val2 - 0.25f));
			note2->MoveTo(1.1f * cosf(val2), 1.1f * sinf(val2), 3.0f);
			rasterizer->AddToRenderList(note2);
		}
		else if(val2 <= 1.0f)
		{
			note2->MoveTo(lerp(0.0f, -2.3f, 4.0f * (val2 - 0.75f)), -1.1f, 3.0f);
			rasterizer->AddToRenderList(note2);
		}
	}
	if(val3 > 0.0f)
	{
		if(val3 <= 0.25f)
		{
			note3->MoveTo(lerp(2.3f, 0.0f, 4.0f * val3), -1.1f, 3.0f);
			rasterizer->AddToRenderList(note3);
		}
		else if(val3 <= 0.75f)
		{
			val3 = lerp(PIBY2 + PI, -PIBY2, 2.0f * (val3 - 0.25f));
			note3->MoveTo(1.1f * cosf(val3), 1.1f * sinf(val3), 3.0f);
			rasterizer->AddToRenderList(note3);
		}
		else if(val3 <= 1.0f)
		{
			note3->MoveTo(lerp(0.0f, -2.3f, 4.0f * (val3 - 0.75f)), -1.1f, 3.0f);
			rasterizer->AddToRenderList(note3);
		}
	}

	graphics->Clear();
	graphics->FillBuffer(rf);
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void hit_ear(float val)
{
	Mesh *note1, *note2, *note3, *ear;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	ear = rm->GetMesh(5);
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	float val1 = val * 4.0f;
	if(val1 <= 1.0f)
	{
		note1->MoveTo(lerp(1.4f, -0.6f, val1), lerp(0.4f, 0.2f, val1), lerp(1.2f, 1.5f, val1));
	}
	else if(val1 <= 3.0f)
	{
		val1  = (val1 - 1.0f) * 0.5f;
		val1 = 1.0f - ((val1 - 1.0f) * (val1 - 1.0f));
		note1->MoveTo(lerp(-0.6f, 0.2f, val1), lerp(0.2f, 1.0f, val1), lerp(1.5f, 2.0f, val1));
	}

	float val2 = (val * 4.0f) - 1.0f;
	if(val2 <= 1.0f)
	{
		note2->MoveTo(lerp(1.4f, -0.6f, val2), lerp(0.4f, 0.2f, val2), lerp(1.2f, 1.5f, val2));
	}
	else if(val2 <= 3.0f)
	{
		val2 = (val2 - 1.0f) * 0.4f;
		val2 = 1.0f - ((val2 - 1.0f) * (val2 - 1.0f));
		note2->MoveTo(lerp(-0.6f, 0.2f, val2), lerp(0.2f, -0.6f, val2), lerp(1.5f, 2.0f, val2));
	}

	float val3 = (val * 4.0f) - 2.0f;
	if(val3 <= 1.0f)
	{
		note3->MoveTo(lerp(1.4f, -0.6f, val3), lerp(0.4f, 0.2f, val3), lerp(1.2f, 1.5f, val3));
	}
	else if(val3 <= 3.0f)
	{
		val3 = (val3 - 1.0f) * 0.5f;
		val3 = 1.0f - ((val3 - 1.0f) * (val3 - 1.0f));
		note3->MoveTo(lerp(-0.6f, 0.6f, val3), 0.2f, lerp(1.5f, 2.3f, val3));
	}

	rasterizer->AddToRenderList(note1);
	rasterizer->AddToRenderList(note2);
	rasterizer->AddToRenderList(note3);

	rasterizer->AddToRenderList(ear);

	graphics->Clear();
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void go_into_ear(float val)
{
	Mesh *note1, *note2, *note3, *ear;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	ear = rm->GetMesh(5);
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	float scaleval;

	float val1 = 2.0f * val;
	val1 *= val1;
	if(val1 <= 0.5f)
	{
		note1->MoveTo(0.2f * cosf(3.0f * PIBY2 - val1 * FOURPI) + 0.2f, 0.2f * sinf(3.0f * PIBY2 - val1 * FOURPI) + 1.2f, 2.0f);
		rasterizer->AddToRenderList(note1);
	}
	else if(val1 <= 1.0f)
	{
		val1 = val1 * 2.0f - 1.0f;
		note1->MoveTo(lerp(0.2f, -0.7f, val1), lerp(1.0f, 0.0f, val1), lerp(2.0f, 1.5f, val1));
		scaleval = (1.0f - val1) * 0.9f + 0.1f;
		note1->ScaleX(scaleval);
		note1->ScaleY(scaleval);
		note1->ScaleZ(scaleval);
		rasterizer->AddToRenderList(note1);
	}

	float val2 = 2.0f * val - 0.5f;
	val2 *= val2;
	if(val >= 0.25f)
	{
		if(val2 <= 0.5f)
		{
			note2->MoveTo(0.2f * cosf(PIBY2 + val2 * FOURPI) + 0.2f, 0.2f * sinf(PIBY2 + val2 * FOURPI) - 0.8f, 2.0f);
			rasterizer->AddToRenderList(note2);
		}
		else if(val2 <= 1.0f)
		{
			val2 = val2 * 2.0f - 1.0f;
			note2->MoveTo(lerp(0.2f, -0.7f, val2), lerp(-0.6f, 0.0f, val2), lerp(2.0f, 1.5f, val2));
			scaleval = (1.0f - val2) * 0.9f + 0.1f;
			note2->ScaleX(scaleval);
			note2->ScaleY(scaleval);
			note2->ScaleZ(scaleval);
			rasterizer->AddToRenderList(note2);
		}
	}
	else
	{
		rasterizer->AddToRenderList(note2);
	}

	float val3 = 2.0f * val - 1.0f;
	val3 *= val3;
	if(val >= 0.5f)
	{
		if(val3 <= 0.5f)
		{
			note3->MoveTo(-0.2f * sinf(val3 * FOURPI) + 0.2f, 0.2f, 2.0f);
			rasterizer->AddToRenderList(note3);
		}
		else if(val3 <= 1.0f)
		{
			val3 = val3 * 2.0f - 1.0f;
			note3->MoveTo(lerp(0.2f, -0.7f, val3), lerp(0.2f, 0.0f, val3), lerp(2.0f, 1.5f, val3));
			scaleval = (1.0f - val3) * 0.9f + 0.1f;
			note3->ScaleX(scaleval);
			note3->ScaleY(scaleval);
			note3->ScaleZ(scaleval);
			rasterizer->AddToRenderList(note3);
		}
	}
	else
	{
		rasterizer->AddToRenderList(note3);
	}

	rasterizer->AddToRenderList(ear);

	graphics->Clear();
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void rotate_zoom_ear(float val)
{
	Mesh *ear;
	ear = rm->GetMesh(5);
	CircleFill *cf = (CircleFill *)rm->GetBackFill(3);
	float *prevval = rm->GetFloat(3);
	float diff = val - *prevval;
	float currval = val;
	bool add_fill = false;
	Light light;

	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	rasterizer->AddToRenderList(ear);

	val *= 2.0f;

	if(val <= 1.0f)
	{
		ear->MoveTo(slerp(-0.7f, 0.0f, val), -0.1f, lerp(1.5f, 2.1f, val));
		ear->RotateAboutY(-PIBY3 * diff);
	}
	else
	{
		val -= 1.0f;
		ear->MoveTo(0.0f, -0.1f, lerp(2.1f, 0.3f, val * val));
		ear->ReColour(Color(1.0f, 1.0f - 0.5f * val, 1.0f - 0.5f * val));
		val *= val;
		if(val > 0.5f)
		{
			add_fill = true;
			cf->SetRadius((val - 0.5f) * 100.0f);
		}
	}

	graphics->Clear();
	rasterizer->Render();
	if(add_fill)
	{
		graphics->FillBuffer(cf);
	}
	graphics->Present();
	rasterizer->ClearRenderList();

	*prevval = currval;
}

void pink_pause(void)
{
	graphics->Clear(Color(1, 0.5f, 0.5f));
	graphics->Present();
}

void start_metaballs(float val)
{
	Metaballs *mb = (Metaballs *)rm->GetBackFill(4);

	mb->SetPhase(val * 35.0f);
	mb->SetBallColor(Color(1.0f, 0.5f + 0.5f * val, 0.5f + 0.5f * val));

	graphics->Clear();
	graphics->FillBuffer(mb);
	graphics->Present();
}

void grow_sphere_metaballs(float val)
{
	Mesh *ball;
	ball = rm->GetMesh(6);
	Metaballs *mb = (Metaballs *)rm->GetBackFill(4);
	Light light;

	light.pos = Vector(-1, -cosf(val * TWOPI), sinf(val * TWOPI));
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	ball->MoveTo(0.0f, 0.0f, lerp(4.0f, 3.5f, val));
	ball->ScaleX(0.75f * val);
	ball->ScaleY(0.75f * val);
	ball->ScaleZ(0.75f * val);

	rasterizer->AddToRenderList(ball);
	rasterizer->SetAmbientLight(0.4f);

	mb->SetPhase(val * 70.0f + 35.0f);

	graphics->Clear();
	graphics->FillBuffer(mb);
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void continue_metaballs(float val)
{
	Mesh *ball;
	ball = rm->GetMesh(6);
	Metaballs *mb = (Metaballs *)rm->GetBackFill(4);
	Light light;

	light.pos = Vector(-1, -cosf(val * TWOPI), sinf(val * TWOPI));
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	rasterizer->AddToRenderList(ball);
	rasterizer->SetAmbientLight(0.4f);

	mb->SetPhase(val * 17.5f + 105.0f);

	graphics->Clear();
	graphics->FillBuffer(mb);
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void swirl_notes_into_metaball(float val)
{
	Mesh *note1, *note2, *note3, *ball;
	note1 = rm->GetMesh(0);
	note2 = rm->GetMesh(1);
	note3 = rm->GetMesh(2);
	ball = rm->GetMesh(6);
	Metaballs *mb = (Metaballs *)rm->GetBackFill(4);
	float *scale = rm->GetFloat(4);
	Light light;

	light.pos = Vector(-1, -cosf(val * TWOPI), sinf(val * TWOPI));
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	rasterizer->AddToRenderList(ball);
	rasterizer->SetAmbientLight(0.4f);

	//grow the ball
	float grow = val * 10.0f;
	if((grow >= 3.0f) && (grow < 4.0f))
	{
		grow -= 3.0f;
		ball->MoveTo(0.0f, 0.0f, 3.5f);
		if(grow <= 0.5f)
		{
			*scale = -1.6f * grow * grow + 1.6f * grow + 1.0f;
		}
		else
		{
			*scale = 0.05f * cosf(TWOPI * (grow - 0.5f)) + 1.35f;
		}
		ball->ScaleX(*scale);
		ball->ScaleY(*scale);
	}

	if((grow >= 6) && (grow < 7.0f))
	{
		grow -= 6.0f;
		ball->MoveTo(0.0f, 0.0f, 3.5f);
		if(grow <= 0.5f)
		{
			*scale = -1.6f * grow * grow + 1.6f * grow + 1.3f;
		}
		else
		{
			*scale = 0.05f * cosf(TWOPI * (grow - 0.5f)) + 1.65f;
		}
		ball->ScaleX(*scale);
		ball->ScaleY(*scale);
	}

	if(grow >= 9.0f)
	{
		grow -= 9.0f;
		ball->MoveTo(0.0f, 0.0f, 3.5f);
		if(grow <= 0.5f)
		{
			*scale = -2.0f * grow * grow + 2.0f * grow + 1.6f;
		}
		else
		{
			*scale = 0.05f * cosf(TWOPI * (grow - 0.5f)) + 2.05f;
		}
		ball->ScaleX(*scale);
		ball->ScaleY(*scale);
	}


	//move the notes
	float interp = val * 10.0f / 3.0f;
	float val1 = interp * -10.0f;
	if(interp <= 1.0f)
	{
		float r = powf(1.5f, val1);
		note1->MoveTo(r * cosf(val1 + PIBY2), r * sinf(val1 + PIBY2), lerp(0.5f, 1.8f, interp));
		note1->ReColour(Color(1.0f, interp, interp));
		rasterizer->AddToRenderList(note1);
	}
	interp = (val * 10.0f - 3.0f) / 3.0f;
	float val2 = interp * -10.0f;
	if((interp >= 0.0f) && (interp <= 1.0f))
	{
		float r = powf(1.5f, val2);
		note2->MoveTo(r * cosf(val2 - PIBY6), r * sinf(val2 - PIBY6), lerp(0.5f, 1.8f, interp));
		note2->ReColour(Color(interp, 1.0f, interp));
		rasterizer->AddToRenderList(note2);
	}
	interp = (val * 10.0f - 6.0f) / 3.0f;
	float val3 = interp * -10.0f;
	if((interp >= 0.0f) && (interp <= 1.0f))
	{
		float r = powf(1.5f, val3);
		note3->MoveTo(r * cosf(val3 + PI + PIBY6), r * sinf(val3 + PI + PIBY6), lerp(0.5f, 1.8f, interp));
		note3->ReColour(Color(interp, interp, 1.0f));
		rasterizer->AddToRenderList(note3);
	}

	mb->SetPhase(val * 43.75f + 122.5f);

	graphics->Clear();
	graphics->FillBuffer(mb);
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void grow_ball_unstable(float val)
{
	Mesh *ball;
	ball = rm->GetMesh(6);
	Metaballs *mb = (Metaballs *)rm->GetBackFill(4);
	Light light;

	light.pos = Vector(-1, -cosf(val * FOURPI), sinf(val * FOURPI));
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	float scale = 2.0f + val + 0.175f * sinf(EIGHTPI * val);
	ball->MoveTo(0.0f, 0.0f, 3.5f);
	ball->ScaleX(scale);
	ball->ScaleY(scale);

	rasterizer->AddToRenderList(ball);
	rasterizer->SetAmbientLight(0.4f);

	mb->SetPhase(val * 70.0f + 166.25f);
	mb->SetBallColor(Color(1.0f, 1.0f - 0.5f * val, 1.0f - 0.5f * val));

	graphics->Clear();
	graphics->FillBuffer(mb);
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();
}

void explode_ball(float val)
{
	Mesh *ball = rm->GetMesh(6);
	float *prevval = rm->GetFloat(5);
	float diff = val - *prevval;
	Light light;


	light.pos = Vector(-1, -cosf(val * FOURPI), sinf(val * FOURPI));
	light.intensity = 1.0f;
	rasterizer->AddToRenderList(&light);

	rasterizer->AddToRenderList(ball);
	rasterizer->SetAmbientLight(0.4f);

	ball->Explode(val * 0.8f, 0.05f);

	if(val < 0.75f)
	{
		val *= (1.0f + THIRD) * 0.5f;
		graphics->Clear(Color(1.0f, 0.5f + val, 0.5f + val));
	}
	else
	{
		val = 4.0f - 4.0f * val;
		ball->ReColour(Color(val, val, val));
		graphics->Clear(Color(val, val, val));
	}
	rasterizer->Render();
	graphics->Present();
	rasterizer->ClearRenderList();

	*prevval = val;
}

void fire_credits(float val, float val2, int stepnum)
{
	Fire *fire = (Fire *)rm->GetBackFill(5);
	fire->SetPhase(val2 * 0.7f);
	graphics->Clear();

	graphics->FillBuffer(fire);

	switch(stepnum)
	{
	case 0:
		font->PrintPhrase("Code:", Color(val, val, val), 5, 5);
		break;
	case 1:
		font->PrintPhrase("Code:", Color(1, 1, 1), 5, 5);
		font->PrintPhrase("Reaper_Unreal", Color(val, val, val), 10, 13);
		break;
	case 2:
		font->PrintPhrase("Code:", Color(1 - val, 1 - val, 1 - val), 5, 5);
		font->PrintPhrase("Reaper_Unreal", Color(1 - val, 1 - val, 1 - val), 10, 13);
		font->PrintPhrase("Music:", Color(val, val, val), 5, 21);
		break;
	case 3:
		font->PrintPhrase("Music:", Color(1, 1, 1), 5, 21);
		font->PrintPhrase("Haz", Color(val, val, val), 20, 28);
		break;
	case 4:
		font->PrintPhrase("Art:", Color(val, val, val), 5, 36);
		val = 1 - val;
		font->PrintPhrase("Music:", Color(val, val, val), 5, 21);
		font->PrintPhrase("Haz", Color(val, val, val), 20, 28);
		break;
	case 5:
		font->PrintPhrase("Art:", Color(1, 1, 1), 5, 36);
		font->PrintPhrase("Reaper_Unreal", Color(val, val, val), 10, 44);
		break;
	case 6:
		val = 1- val;
		font->PrintPhrase("Art:", Color(val, val, val), 5, 36);
		font->PrintPhrase("Reaper_Unreal", Color(val, val, val), 10, 44);
		break;
	default: break;
	}

	graphics->Present();
}

void go_fire(float val)
{
	Fire *fire = (Fire *)rm->GetBackFill(5);
	graphics->Clear();
	fire->SetPhase(val * 0.7f);
	graphics->FillBuffer(fire);
	graphics->Present();
}

void fire_die(void)
{
	Fire *fire = (Fire *)rm->GetBackFill(5);
	graphics->Clear();
	fire->SetSource(false);
	graphics->FillBuffer(fire);
	graphics->Present();
}

void demo_main(void)
{
	//in usual C-style declare all the needed variables at the beginning
	bool done = false;
	int tick = 0, songtime, frame = 0, effectnum = -1;
	int timestep = lrint(1000.0/24.0);
	float effectval = 0.0f;
	float effectstepval = 0.0f;
	float effectval2 = 0.0f;
	float effectstepval2 = 0.0f;

	//get all the resources we'll need
	init_resources();

	//wait a bit for start
	Sleep(1500);

	//start up the song
	FSOUND_Stream_Play(0, song);

	//set a decent volume
	FSOUND_SetVolume(FSOUND_ALL, 255);

	//demo goes in here
	while(KeyUp(VK_ESCAPE) && !done)
	{
		//check for done
		if(frame > 2444)
		{
			break;
		}

		//sync demo to song
		while(tick >= FSOUND_Stream_GetTime(song))
		{
			Sleep(0);
		}

		//frameskip and effect setup
		songtime = FSOUND_Stream_GetTime(song);
		while(tick < songtime)
		{
			//swicth between effects based on frame
			switch(frame)
			{
			case 12: //T
				effectnum = 0;
				break;
			case 24: //M
				effectnum = 1;
				break;
			case 36: //D
				effectnum = 2;
				break;
			case 48: //C
				effectnum = 3;
				break;
			case 60: //8
				effectval = 0.0f;
				effectnum = 4;
				effectstepval = 1.0f / 12.0f;
				break;
			case 72: //stop 8 fade
				effectval = 1.0f;
				effectstepval = 0.0f;
				break;
			case 100:
				effectnum = 5;
				effectval = 0.0f;
				effectstepval = 1.0f / 10.0f;
				break;
			case 110:
				effectnum = 6;
				effectval = 1.0f;
				effectstepval = -1.0f / 10.0f;
				break;
			case 120:
				effectnum = -1;
				effectval = 0.0f;
				effectstepval = 0.0f;
				break;
			case 133:
				effectnum = 7;
				effectval = 0.0f;
				effectstepval = 1.0f / 10.0f;
				break;
			case 143:
				effectnum = 8;
				effectval = 0.0f;
				effectstepval = 1.0f / 10.0f;
				break;
			case 153:
				effectnum = 9;
				effectval = 0.0f;
				effectstepval = 1.0f / 11.0f; //11
				break;
			case 164:
				effectnum = 10;
				effectval = 0.0f;
				effectstepval = 1.0f / 10.0f;
				break;
			case 174:
				effectnum = 11;
				effectval = 0.0f;
				break;
			case 184:
				effectnum = 12;
				effectval = 0.0f;
				effectstepval = 1.0f / 11.0f; //11
				break;
			case 193:
				effectnum = 13;
				effectval = 0.0f;
				effectstepval = 1.0f / 10.0f;
				break;
			case 203:
				effectnum = 14;
				effectval = 0.0f;
				break;
			case 213:
				effectnum = 15;
				effectval = 0.0f;
				effectstepval = 1.0f / 11.0f; //11
				break;
			case 224:
				effectnum = 16;
				effectval = 0.0f;
				effectstepval = 1.0f / 10.0f; //20
				break;
			case 244:
				effectnum = 17;
				effectval = 0.0f;
				effectstepval = 1.0f / 21.0f; //21
				break;
			case 265:
				effectnum = 18;
				effectval = 0.0f;
				effectstepval = 1.0f / 21.0f; //21
				break;
			case 286:
				effectnum = 19;
				effectval = 0.0f;
				effectstepval = 1.0f / 20.0f; //20
				break;
			case 306:
				effectnum = 20;
				effectval = 0.0f;
				effectstepval = 1.0f / 31.0f; //31
				break;
			case 337:
				effectnum = 21;
				effectval = 0.0f;
				effectstepval = 1.0f / 20.0f; //20
				break;
			case 357:
				effectnum = 22;
				effectval = 0.0f;
				effectstepval = 1.0f / 83.0f; //166 (16 beats) ends on 11
				break;
			case 523:
				effectnum = 23;
				effectval = 0.0f;
				effectstepval = 1.0f / 82.0f; //82 (8 beats) ends on second 10
				break;
			case 605:
				effectnum = 24;
				effectval = 0.0f;
				effectstepval = 1.0f / 83.0f; //83 (8 beats) ends on first 10
				break;
			case 688:
				effectnum = 25;
				effectval = 0.0f;
				effectstepval = 1.0f / 165.0f; //165 (16 beats)ends on second 10
				break;
			case 853:
				effectnum = 26;
				effectval = 0.0f;
				effectstepval = 1.0f / 83.0f; //83 (8 beats) ends on first 10
				break;
			case 936:
				effectnum = 27;
				effectval = 0.0f;
				effectstepval = 1.0f / 21.0f; //21 (2 beats) ends on 11
				break;
			case 957:
				effectnum = 28;
				effectval = 0.0f;
				effectstepval = 1.0f / 20.0f; //20 (2 beats) ends on second 10
				break;
			case 977:
				effectnum = 29;
				effectval = 0.0f;
				effectstepval = 1.0f / 42.0f; //42 (4 beats) ends on 11
				break;
			case 1019:
				effectnum = 30;
				effectval = 0.0f;
				effectstepval = 1.0f / 41.0f; //41 (4 beats) ends on first 10
				break;
			case 1060:
				effectnum = 31;
				effectval = 0.0f;
				effectstepval = 1.0f / 41.0f; //41 (4 beats) ends on second 10
				break;
			case 1101:
				effectnum = 32;
				effectval = 0.0f;
				effectstepval = 1.0f / 42.0f; //42 (4 beats) ends on 11
				break;
			case 1143:
				effectnum = 33;
				effectval = 0.0f;
				effectstepval = 1.0f / 72.0f; //72 (7 beats) ends on first 10
				break;
			case 1215:
				effectnum = 34;
				effectval = 0.0f;
				effectstepval = 1.0f / 41.0f; //42 (4 beats) ends on second 10
				recolour_notes();
				break;
			case 1256:
				effectnum = 35;
				effectval = 0.0f;
				effectstepval = 1.0f / 83.0f; //83 (8 beats) ends on first 10
				break;
			case 1339:
				effectnum = 36;
				effectval = 0.0f;
				effectstepval = 1.0f / 83.0f; //83 (8 beats) ends on 11
				break;
			case 1442:
				effectnum = 37;
				effectval = 0.0f;
				effectstepval = 1.0f / 41.0f; //41 (4 beats) ends on first 10
				break;
			case 1483:
				effectnum = 38;
				effectval = 0.0f;
				effectstepval = 1.0f / 41.0f; //41 (4 beats) ends on second 10
				break;
			case 1524:
				effectnum = 39;
				effectval = 0.0f;
				effectstepval = 1.0f / 11.0f; //11 (1 beat) spacing, ends on 11
				break;
			case 1535:
				effectnum = 40;
				effectval = 0.0f;
				effectstepval = 1.0f / 82.0f; //82 (8 beats) ends on second 10
				break;
			case 1617:
				effectnum = 41;
				effectval = 0.0f;
				effectstepval = 1.0f / 166.0f; //166 (16 beats) ends on 11
				break;
			case 1783:
				effectnum = 42;
				effectval = 0.0f;
				effectstepval = 1.0f / 41.0f; //41 (4 beats) ends on first 10
				break;
			case 1824:
				effectnum = 43;
				effectval = 0.0f;
				effectstepval = 1.0f / 103.0f; //103 (10 beats) ends on second 10
				break;
			case 1927:
				effectnum = 44;
				effectval = 0.0f;
				effectstepval = 1.0f / 166.0f; //166 (16 beats) ends on 11
				break;
			case 2093:
				effectnum = 45;
				effectval = 0.0f;
				effectstepval = 1.0f / 82.0f; //82 (8 beats) ends on second 10
				break;
			case 2175:
				effectnum = 46;
				effectval2 = 0.0f;
				effectstepval2 = 1.0f / 10.0f; //just continue this 'til the end
				effectval = 0.0f;
				effectstepval = 1.0f / 21.0f; //21 (2 beats) ends on first 10
				break;
			case 2196:
				effectnum = 47;
				effectval = 0.0f;
				effectstepval = 1.0f / 21.0f; //21 (2 beats) ends on 11
				break;
			case 2217:
				effectnum = 48;
				effectval = 0.0f;
				effectstepval = 1.0f / 20.0f; //20 (2 beats) ends on second 10
				break;
			case 2237:
				effectnum = 49;
				effectval = 0.0f;
				effectstepval = 1.0f / 21.0f; //21 (2 beats) ends on first 10
				break;
			case 2258:
				effectnum = 50;
				effectval = 0.0f;
				effectstepval = 1.0f / 21.0f; //21 (2 beats) ends on 11
				break;
			case 2279:
				effectnum = 51;
				effectval = 0.0f;
				effectstepval = 1.0f / 20.0f; //20 (2 beats) ends on second 10
				break;
			case 2299:
				effectnum = 52;
				effectval = 0.0f;
				effectstepval = 1.0f / 21.0f; //21 (2 beats) ends on first 10
				break;
			case 2320:
				effectnum = 53;
				effectstepval = 0.0f; //62 (6 beats) ends on first 10
				break;
			case 2382:
				effectnum = 54;
				effectstepval = 0.0f;
				effectstepval2 = 0.0f; //62 (6 beats) ends on first 10
				break;
			case 2444:
				effectnum = -3;
				break;
			default: break;
			}

			//increase the frame and time
			//framerate locked at 24fps
			tick += timestep;
			frame++;

			//increase the effect values
			effectval += effectstepval;
			effectval2 += effectstepval2;
		}

		//execute the effect chosen
		switch(effectnum)
		{
		case -3:
			done = true;
			break;
		case -2:
			black_pause();
			break;
		case -1:
			break; //don't change anything
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			text_intro(effectval, effectnum);
			break;
		case 6:
			fade_from_white(effectval);
			break;
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
			bounce_notes(effectnum - 7, effectval);
			break;
		case 13:
		case 14:
			bounce_notes(5, effectval);
			break;
		case 15:
			bounce_notes(6, effectval);
			break;
		case 16:
			twirl_notes(effectval);
			break;
		case 17:
			fly_away_notes(effectval);
			break;
		case 18:
			zoom_to_notes(effectval);
			break;
		case 19:
			prepare_for_swirl(effectval);
			break;
		case 20:
			swirl_note(effectval);
			break;
		case 21:
			fade_in_tunnel(effectval);
			break;
		case 22:
			move_tunnel(effectval);
			break;
		case 23:
			tunnel_pulse(effectval);
			break;
		case 24:
			tunnel_circle_forward(effectval);
			break;
		case 25:
			tunnel_fade_gold(effectval);
			break;
		case 26:
			move_notes_golden_tunnel(effectval);
			break;
		case 27:
			pause_tunnel(effectval);
			break;
		case 28:
			fade_out_notes(effectval);
			break;
		case 29:
		case 30:
		case 31:
		case 32:
		case 33:
			show_instrument(effectval, effectnum - 29);
			break;
		case 34:
			notes_fly_out(effectval);
			break;
		case 35:
			notes_circle_across_screen(effectval);
			break;
		case 36:
			hit_ear(effectval);
			break;
		case 37:
			go_into_ear(effectval);
			break;
		case 38:
			rotate_zoom_ear(effectval);
			break;
		case 39:
			pink_pause();
			break;
		case 40:
			start_metaballs(effectval);
			break;
		case 41:
			grow_sphere_metaballs(effectval);
			break;
		case 42:
			continue_metaballs(effectval);
			break;
		case 43:
			swirl_notes_into_metaball(effectval);
			break;
		case 44:
			grow_ball_unstable(effectval);
			break;
		case 45:
			explode_ball(effectval);
			break;
		case 46:
		case 47:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
			fire_credits(effectval, effectval2, effectnum - 46);
			break;
		case 53:
			go_fire(effectval2);
			break;
		case 54:
			fire_die();
			break;
		default: break;
		}
	}

	//stop the song
	FSOUND_Stream_Stop(song);

	//clear out our store of stuff
	clear_resources();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	console = new Console("Music To My Ears - Computing Color Map");
	graphics = new Graphics(*console);
	console->setTitle("Music To My Ears - Press alt + enter for fullscreen");
	rasterizer = new Rasterizer(graphics);
	rasterizer->SetFOV(45);
	rasterizer->SetRenderMode(GOURAD);
	font = new Font();
	rm = new ResourceManager();
	if(!font->LoadFont("Data/font.dat"))
	{
		//big problem here!
		graphics->Clear();
		graphics->WriteText("Error Loading Font", 18, 30, 18, 1, 0, 0);
		graphics->Present();
		Sleep(1000);
		return 0;
	}
	/*if(!FSOUND_SetHWND(console->getConsoleHandle()))
	{
		//sound problem
		graphics->Clear();
		graphics->WriteText("Error Initializing FMOD", 23, 14, 18, 1, 0, 0);
		graphics->Present();
		Sleep(1000);
		return 0;
	}*/
	if(!FSOUND_Init(44100, 2, 0))
	{
		//sound problem
		graphics->Clear();
		graphics->WriteText("Error Initializing FMOD", 23, 14, 18, 1, 0, 0);
		graphics->Present();
		Sleep(1000);
		return 0;
	}
	if(!(song = FSOUND_Stream_Open("Data/synth_intro_song.ogg", 0, 0, 0)))
	{
		//sound problem
		graphics->Clear();
		graphics->WriteText("Error Loading Song", 18, 26, 18, 1, 0, 0);
		graphics->Present();
		Sleep(1000);
		return 0;
	}
	FSOUND_Stream_SetLoopCount(song, 0);
	demo_main();
	/*float phase = 0.0f;
	Light light, light2;
	light.pos = Vector(-1, -1, -1);
	light.intensity = 1.0f;
	light2.pos = Vector(1, 0.5f, -0.5f);
	light2.intensity = 0.5f;
	Mesh *cube = new Mesh();
	cube->ReadFrom("cube.gmf");
	cube->ReColour(Color(1, 0, 0));
	cube->Translate(0, 0, 2.1f);
	ColorFill cf(Color(0.9f, 0, 0));
	AnimatedCircle ac(graphics->GetColorMap());
	//Tunnel tu(graphics->GetColorMap());
	Metaballs mb;
	FSOUND_Stream_Play(0, song);
	while(!KeyDown(VK_ESCAPE))
	{
		graphics->Clear();
		//graphics->FillBuffer(&tu);
		//light.pos = Vector(cosf(phase), sinf(phase), -1);
		rasterizer->AddToRenderList(cube);
		rasterizer->AddToRenderList(&light);
		//rasterizer->AddToRenderList(&light2);
		rasterizer->Render();
		//char numtrisphrase[100];
		//sprintf(numtrisphrase, "Polygons: %d", cube->GetNumTris());
		//font->PrintPhrase(numtrisphrase, Color(1, 1, 1), 2, 2);
		//font->PrintPhrase("rotating cube!", Color((0.5f * sinf(phase)) + 0.5f, (0.5f * cosf(phase)) + 0.5f, (0.5f * cosf(phase + 0.5f)) + 0.5f), 10, 10);
		graphics->Present();
		rasterizer->ClearRenderList();
		phase += 0.1f;
		Sleep(33);
		if(KeyDown(VK_F1))
		{
			cube->ScaleX(1.05f);
			cube->ScaleY(1.05f);
			cube->ScaleZ(1.05f);
		}
		if(KeyDown(VK_F2))
		{
			cube->ScaleX(0.95f);
			cube->ScaleY(0.95f);
			cube->ScaleZ(0.95f);
		}
		if(KeyDown(VK_DOWN))
		{
			cube->RotateAboutX(-0.05f);
		}
		if(KeyDown(VK_UP))
		{
			cube->RotateAboutX(0.05f);
		}
		if(KeyDown(VK_LEFT))
		{
			cube->RotateAboutY(-0.05f);
		}
		if(KeyDown(VK_RIGHT))
		{
			cube->RotateAboutY(0.05f);
		}
		if(KeyDown(0x31))
		{
			if(shape != 1)
			{
				cube->ReadFrom("cube.gmf");
				shape = 1;
				cube->ReColour(Color(1, 0, 0));
				cube->Translate(0, 0, 2.1f);
			}
		}
		if(KeyDown(0x32))
		{
			if(shape != 2)
			{
				cube->ReadFrom("torus.gmf");
				shape = 2;
				cube->ReColour(Color(0, 1, 0));
				cube->Translate(0, 0, 2.1f);
			}
		}
		if(KeyDown(0x33))
		{
			if(shape != 3)
			{
				cube->ReadFrom("teapot.gmf");
				shape = 3;
				cube->ReColour(Color(0, 0, 1));
				cube->Translate(0, 0, 2.1f);
			}
		}
		if(KeyDown(0x34))
		{
			if(shape != 4)
			{
				cube->ReadFrom("knot.gmf");
				shape = 4;
				cube->ReColour(Color(1, 1, 1));
				cube->Translate(0, 0, 250.0f);
			}
		}
		if(KeyDown(0x35))
		{
			if(shape != 5)
			{
				cube->ReadFrom("note.gmf");
				shape = 5;
				cube->ReColour(Color(1, 0, 1));
				cube->Translate(0, 0, 1.8f);
			}
		}
		if(KeyDown(0x36))
		{
			if(shape != 6)
			{
				cube->ReadFrom("trumpet.gmf");
				shape = 6;
				cube->ReColour(Color(1, 1, 0));
				cube->Translate(0, 0, 4.0f);
			}
		}
		while(KeyDown(VK_SPACE))
		{
			Sleep(60);
		}
	}
	delete cube;*/
	delete rm;
	FSOUND_Stream_Close(song);
	FSOUND_Close();
	delete font;
	delete rasterizer;
	delete graphics;
	delete console;
	return 0;
}