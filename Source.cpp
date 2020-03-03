#include <iostream>
#include <vector>
using namespace std;

class Produs {
	string unit_de_masura;
	string denumire;
	int cant;
public:
	Produs() { unit_de_masura = ""; denumire = ""; cant = 0; }
	Produs(string a, string b, int c) { unit_de_masura = a; denumire = b; cant = c; }
	Produs(const Produs& ob);
	Produs& operator= (const Produs& ob);
	friend istream& operator>> (istream&, Produs*);
	friend ostream& operator<< (ostream&, Produs&);
	virtual void afisare(ostream& out)
	{
		out << "Denumire produs: " << denumire << "\nUnitatea de masurra: " << unit_de_masura << "\nCantitatea: " << cant << endl;
	}

};

class Produs_perisabil: virtual public Produs {
	int per_val;
public:
	Produs_perisabil() { per_val = 0; }
	Produs_perisabil(string a, string b, int c, int d) : Produs(a, b, c) { per_val = d; }
	Produs_perisabil(const Produs_perisabil& ob) : Produs(ob) { per_val = ob.per_val; }
	Produs_perisabil& operator= (const Produs_perisabil&);
	friend istream& operator>> (istream&, Produs_perisabil*);
	friend ostream& operator<< (ostream&, Produs_perisabil&);
	void afisare(ostream& out)
	{
		Produs::afisare(out);
		out << endl << "Per val: " << per_val;
	}

};

class Produs_red : virtual public Produs {
	int dis;
public:
	Produs_red() { dis = 0; }
	Produs_red(string a, string b, int c, int d) : Produs(a, b, c) { dis = d; }
	Produs_red(const Produs_red& ob) : Produs(ob) { dis = ob.dis; }
	Produs_red& operator= (const Produs_red& ob);
	friend istream& operator>> (istream&, Produs_red*);
	friend ostream& operator<< (ostream&, Produs_red&);
	void afisare(ostream& out)
	{
		Produs::afisare(out);
		out << endl << "Reducere: " << dis;
	}
	void set_disc(int a)
	{
		this->dis = a;
	}
	int get_disc()
	{
		return this->dis;
	}

};

class Produs_pers_disc : public Produs_red, public Produs_perisabil
{
public:
	Produs_pers_disc() {};
	Produs_pers_disc(string a, string b, int c, int d, int e) : Produs_red(a, b, c, e), Produs_perisabil(a, b, c, d) {}
	Produs_pers_disc& operator=(const Produs_pers_disc&);
	~Produs_pers_disc()
	{
		cout << "\nDestruc prod_pers_disc\n";
	}
	void set_disc(int a)
	{
		this->Produs_red::set_disc(a);
	}
	int get_disc()
	{
		return this->Produs_red::get_disc();
	}
	friend istream& operator>> (istream& in, Produs_pers_disc*);
	friend ostream& operator<< (ostream& out, Produs_pers_disc&);
	void afisare(ostream& out)
	{
		Produs_perisabil::afisare(out);
		int a = get_disc();
		out << "\n DIScount de: " << a;
	}

};

class Gestiune
{
	vector<Produs*> lista;
	int index;
public:
	Gestiune(int a = 0)
	{
		index = a;
	}
	Gestiune(const Gestiune& ob)
	{
		this->index = ob.index;
		lista.clear();
		for (int i = 0; i < index; i++)
		{
			Produs_red* p1 = dynamic_cast<Produs_red*>(ob.lista[i]);
			if (p1 != NULL)
			{
				lista.push_back(new Produs_red(*p1));
			}
			else
			{

				Produs_perisabil* p2 = dynamic_cast <Produs_perisabil*> (ob.lista[i]);
				if (p2 != NULL)
				{
					lista.push_back(new Produs_perisabil(*p2));
				}
				else
				{
					Produs_pers_disc* p3 = dynamic_cast <Produs_pers_disc*>(ob.lista[i]);
				}
			}

		}

	}
	Gestiune& operator= (const Gestiune& ob)
	{
		if (this != &ob)
		{


			this->index = ob.index;
			lista.clear();
			for (int i = 0; i < index; i++)
			{
				Produs_red* p1 = dynamic_cast<Produs_red*>(ob.lista[i]);
				if (p1 != NULL)
				{
					lista.push_back(new Produs_red(*p1));
				}
				else
				{

					Produs_perisabil* p2 = dynamic_cast <Produs_perisabil*> (ob.lista[i]);
					if (p2 != NULL)
					{
						lista.push_back(new Produs_perisabil(*p2));
					}
					else
					{
						Produs_pers_disc* p3 = dynamic_cast <Produs_pers_disc*>(ob.lista[i]);
					}
				}

			}
		}
	}
	Gestiune& operator+=(Produs& ob)
	{
		index++;
		lista.push_back(&ob);
		return *this;
	}

	~Gestiune() { lista.clear(); }
	friend ostream& operator<< (ostream& out, Gestiune& ob)
	{
		out << "Nr de loturi: " << ob.index;
		for (int i = 0; i < ob.index; i++)
		{
			out << "Index lot: " << i << endl << *ob.lista[i];
			out << endl;
		}
	}
};



Produs_pers_disc& Produs_pers_disc:: operator= (const Produs_pers_disc& ob)
{
	if (this != &ob)
	{
		this->Produs_red::operator=(ob);
		this->Produs_perisabil::operator=(ob);
	}
	return *this;
}

istream& operator>>(istream& in, Produs_pers_disc* ob)
{
	in >> (Produs_perisabil*)ob;
	cout << "Dati disc: ";
	int x;
	cin >> x;
	ob->set_disc(x);
	return in;
}

ostream& operator<< (ostream& out, Produs_pers_disc& ob)
{
	ob.afisare(out);
	return out;
}


Produs_red& Produs_red:: operator= (const Produs_red& ob)
{
	if (this != &ob)
	{
		this->Produs::operator=(ob);
		dis = ob.dis;
	}
	return *this;

}

istream& operator>> (istream& in, Produs_red* ob)
{
	in >> (Produs*)ob;
	cout << "\nDati reducere: ";
	in >> ob->dis;
	return in;
}

ostream& operator<< (ostream& out, Produs_red& ob)
{
	ob.afisare(out);
	return out;

}

Produs_perisabil& Produs_perisabil:: operator= (const Produs_perisabil& ob)
{
	if (this != &ob)
	{
		this->Produs::operator=(ob);
		per_val = ob.per_val;
	}
	return *this;
}

istream& operator>>(istream& in, Produs_perisabil* ob)
{
	in >> (Produs*)ob;
	cout << "\nDati termen valab";
	in >> ob->per_val;
	return in;

}

ostream& operator<<(ostream& out, Produs_perisabil& ob)
{
	ob.afisare(out);
	return out;
}


Produs::Produs(const Produs& ob)
{
	unit_de_masura = ob.unit_de_masura;
	denumire = ob.denumire;
	cant = ob.cant;
}

Produs& Produs:: operator= (const Produs& ob)
{
	if (this != &ob)
	{
		unit_de_masura = ob.unit_de_masura;
		denumire = ob.denumire;
		cant = ob.cant;

	}
	return *this;

}

istream& operator>> (istream& in, Produs* ob)
{
	cout << "Dati denumirea: ";
	in >> ob->denumire;
	cout << "\nDati unitatea de masura: ";
	in >> ob->unit_de_masura;
	cout << "\nDati cantitatea: ";
	in >> ob->cant;
	return in;

}

ostream& operator<< (ostream& out, Produs& ob)
{
	ob.afisare(out);
	return out;

}

int main()
{
	Gestiune A;
	int n;
	cout << "Dati nr de loturi: ";
	cin >> n;
	for (int i = 1; i <= n; i++)
	{
		int opt;
		cout << "Opt 1: produs perisabil, Opt 2: produs cu discout, Opt3: Produs perisabil cu disc: ";
		cout << "\n Introduceti opt: ";
		cin >> opt;
		try {
			if (opt != 1 && opt != 2 && opt != 3)
			{
				throw opt;
			}
		}
		catch (int a)
		{
			cout << "Optiune invalida";
		}

		if (opt == 1)
		{
			Produs_perisabil* a = new Produs_perisabil;
			cin >> a;
			A += *a;
		}
		if (opt == 2)
		{
			Produs_red* b = new Produs_red;
			cin >> b;
			A += *b;
		}
		if (opt == 3)
		{
			Produs_pers_disc* c = new Produs_pers_disc;
			cin >> c;
			A += *c;
		}
	}
	cout << A;

}