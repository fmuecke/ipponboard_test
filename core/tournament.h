#ifndef BASE__TOURNAMENT_H_
#define BASE__TOURNAMENT_H_

#include <QString>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <vector>

#include "score.h"

namespace Ipponboard
{
enum { eTournament_FightCount = 10 };
enum EScore
{
	eScore_Ippon = 10,
	eScore_Wazaari = 7,
	eScore_Yuko = 5,
	eScore_Hantai = 1,
	eScore_Hikewake = 0,
	eScore_Lost = 0
};

struct SimpleFighter
{
	QString name;
	QString club;
};

struct Fight
{
	Fight() : weight("-"), time_in_seconds(0), is_saved(false)
	{
	}

	Score scores[2];
    SimpleFighter fighters[2];
	QString weight;
	int time_in_seconds;
	bool is_saved;

	const QString GetRoundTimeRemainingText() const
	{
		// get time display
		const int minutes = time_in_seconds / 60;
		const int seconds = time_in_seconds - minutes * 60;
		QString ret = QString::number(minutes) + ":";

		if (seconds < 10)	// append leading zero?
			ret += "0";

		return  ret + QString::number(seconds);
	}

	const QString GetRoundTimeUsedText(int max_time_in_seconds) const
	{
		// get time display
		const int time_used = max_time_in_seconds - time_in_seconds;
		const int minutes = time_used / 60;
		const int seconds = time_used - minutes * 60;
		QString ret = QString::number(minutes) + ":";
		if (seconds < 10)	// append leading zero?
			ret += "0";
		return  ret + QString::number(seconds);
	}

	bool HasWon(EFighter who) const
	{
		const EFighter tori = who;
		const EFighter uke = (tori == eFighter_Blue) ?
							 eFighter_White : eFighter_Blue;

		if (scores[tori].Ippon() || scores[tori].IsAwaseteIppon())
			return true;

		if (scores[uke].Ippon() || scores[uke].IsAwaseteIppon())
			return false;

		if (scores[tori].Wazaari() > scores[uke].Wazaari())
			return true;

		if (scores[tori].Wazaari() < scores[uke].Wazaari())
			return false;

		if (scores[tori].Yuko() > scores[uke].Yuko())
			return true;

		return false;
	}

	int ScorePoints(EFighter who) const
	{
		const EFighter tori = who;
		const EFighter uke = (tori == eFighter_Blue) ?
							 eFighter_White : eFighter_Blue;

		if (HasWon(tori))
		{
			if (scores[tori].Ippon() || scores[tori].IsAwaseteIppon())
				return eScore_Ippon;

			// Only the fight deciding point is taken into account!
			if (scores[tori].Wazaari() > 0 &&
					scores[tori].Wazaari() > scores[uke].Wazaari())
				return eScore_Wazaari;

			if (scores[tori].Yuko() > 0)
				return eScore_Yuko;

			//TODO: Hantei!
		}
		else
		{
			if (!HasWon(uke))
				return eScore_Hikewake;
		}

		return eScore_Lost;
	}
};

//FIXME:typedef std::vector<Fight> Tournament;
typedef boost::array<Fight, eTournament_FightCount> Tournament;

} // namespace Ipponboard


namespace boost
{
namespace serialization
{
template<class Archive>
void serialize(Archive& ar,
               Ipponboard::SimpleFighter& f,
			   const unsigned int /*version*/)
{
	ar& BOOST_SERIALIZATION_NVP(f.name);
	ar& BOOST_SERIALIZATION_NVP(f.club);
}

template<class Archive>
void serialize(Archive& ar,
			   Ipponboard::Fight& m,
			   const unsigned int /*version*/)
{
	ar& BOOST_SERIALIZATION_NVP(m.scores);
	ar& BOOST_SERIALIZATION_NVP(m.fighters);
	ar& BOOST_SERIALIZATION_NVP(m.weight);
	ar& BOOST_SERIALIZATION_NVP(m.time_in_seconds);
}

template<class Archive>
void serialize(Archive& ar,
			   Ipponboard::Score& s,
			   const unsigned int /*version*/)
{
	ar& BOOST_SERIALIZATION_NVP(s._points);
}

} // namespace serialization
} // namespace boost


#endif  // BASE__TOURNAMENT_H_
