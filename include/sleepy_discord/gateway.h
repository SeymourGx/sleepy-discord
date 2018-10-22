#pragma once
#include <ctime>
#include <array>
#include "discord_object_interface.h"
#include "user.h"
#include "channel.h"
#include "server.h"

namespace SleepyDiscord {
	enum Status {
		statusError = 0,
		online         ,
		doNotDisturb   ,
		idle           ,
		invisuble      ,
		offline        
	};

	enum GameType {
		Playing = 0,  //calling this Game causes issues
		Streaming = 1
	};

	struct Game : DiscordObject {
		std::string name = "";
		GameType type;
		std::string url = ""; //used when type is Streaming
	};

	struct Ready : public DiscordObject {
	public:
		Ready() = default;
		//Ready(const std::string * rawJSON);
		Ready(const json::Value & rawJSON);
		Ready(const nonstd::string_view & rawJSON);
		//Ready(const json::Values values);
		int v;	//gateway protocol version
		User user;
		std::list<Channel> privateChannels;
		std::list<UnavailableServer> servers;
		std::string sessionID;
		//std::vector<std::string> trace;

		JSONStructStart
			std::make_tuple(
				json::pair                           (&Ready::v              , "v"               , json::REQUIRIED_FIELD),
				json::pair                           (&Ready::user           , "user"            , json::REQUIRIED_FIELD),
				json::pair<json::ContainerTypeHelper>(&Ready::privateChannels, "private_channels", json::REQUIRIED_FIELD),
				json::pair<json::ContainerTypeHelper>(&Ready::servers        , "guilds"          , json::REQUIRIED_FIELD),
				json::pair                           (&Ready::sessionID      , "session_id"      , json::REQUIRIED_FIELD)
				//This doesn't work anymore
				//json::pair(&Ready::trace          , json::toArray<std::string>      , "_trace"          , json::REQUIRIED_FIELD)
			);
		JSONStructEnd
	};

	struct ActivityTimestamp {
	public:
		ActivityTimestamp() = default;
		~ActivityTimestamp() = default;
		ActivityTimestamp(const json::Value & json);
		ActivityTimestamp(const nonstd::string_view & json);
		std::time_t start;
		std::time_t end;

		JSONStructStart
			std::make_tuple(
				json::pair(&ActivityTimestamp::start, "start", json::OPTIONAL_FIELD),
				json::pair(&ActivityTimestamp::end  , "end"  , json::OPTIONAL_FIELD)
			);
		JSONStructEnd
	};

	struct ActivityParty {
	public:
		ActivityParty() = default;
		~ActivityParty() = default;
		ActivityParty(const json::Value & json);
		ActivityParty(const nonstd::string_view & json);
		std::string ID;
		std::array<int64_t, 2> size;
		int64_t& currentSize = size[0];
		int64_t& maxSize = size[1];

		JSONStructStart
			std::make_tuple(
				json::pair                           (&ActivityParty::ID  , "id"  , json::OPTIONAL_FIELD),
				json::pair<json::ContainerTypeHelper>(&ActivityParty::size, "size", json::OPTIONAL_FIELD)
			);
		JSONStructEnd
	};

	struct ActivityAssets {
	public:
		ActivityAssets() = default;
		~ActivityAssets() = default;
		ActivityAssets(const json::Value & json);
		ActivityAssets(const nonstd::string_view & json);
		std::string largeImage;
		std::string largeText;
		std::string smallImage;
		std::string smallText;

		JSONStructStart
			std::make_tuple(
				json::pair(&ActivityAssets::largeImage, "large_image", json::OPTIONAL_FIELD),
				json::pair(&ActivityAssets::largeText , "large_text" , json::OPTIONAL_FIELD),
				json::pair(&ActivityAssets::smallImage, "small_image", json::OPTIONAL_FIELD),
				json::pair(&ActivityAssets::smallText , "small_text" , json::OPTIONAL_FIELD)
			);
		JSONStructEnd
	};

	struct ActivitySecrets {
	public:
		ActivitySecrets() = default;
		~ActivitySecrets() = default;
		ActivitySecrets(const json::Value & json);
		ActivitySecrets(const nonstd::string_view & json);
		std::string join;
		std::string spectate;
		std::string match;

		JSONStructStart
			std::make_tuple(
				json::pair(&ActivitySecrets::join    , "join"    , json::OPTIONAL_FIELD),
				json::pair(&ActivitySecrets::spectate, "spectate", json::OPTIONAL_FIELD),
				json::pair(&ActivitySecrets::match   , "match"   , json::OPTIONAL_FIELD)
			);
		JSONStructEnd
	};

	//This is here for the snowflake
	struct Application {};

	struct Activity : public DiscordObject {
	public:
		Activity() = default;
		~Activity() = default;
		Activity(const json::Value & json);
		Activity(const nonstd::string_view & json);
		std::string name;
		enum ActivityType {
			ACTIVITY_TYPE_NONE = -1,
			GAME               = 0,
			STREAMING          = 1,
			LISTENING          = 2,
		} type = ACTIVITY_TYPE_NONE;
		std::string url;
		ActivityTimestamp timestamps;
		Snowflake<Application> applicationID;
		std::string details;
		std::string state;
		ActivityParty party;
		ActivityAssets assets;
		ActivitySecrets secrets;
		bool instance;
		enum ActivityFlags {
			NONE         = 0 << 0,
			INSTANCE     = 1 << 0,
			JOIN         = 1 << 1,
			SPECTATE     = 1 << 2,
			JOIN_REQUEST = 1 << 3,
			SYNC         = 1 << 4,
			PLAY         = 1 << 5
		} flags = NONE;

		JSONStructStart
			std::make_tuple(
				json::pair(&Activity::name         , "name"          , json::REQUIRIED_FIELD        ),
				json::pair(&Activity::type         , "type"          , json::REQUIRIED_FIELD        ),
				json::pair(&Activity::url          , "url"           , json::OPTIONAL_NULLABLE_FIELD),
				json::pair(&Activity::timestamps   , "timestamps"    , json::OPTIONAL_FIELD         ),
				json::pair(&Activity::applicationID, "application_id", json::OPTIONAL_FIELD         ),
				json::pair(&Activity::details      , "details"       , json::OPTIONAL_NULLABLE_FIELD),
				json::pair(&Activity::state        , "state"         , json::OPTIONAL_NULLABLE_FIELD),
				json::pair(&Activity::party        , "party"         , json::OPTIONAL_FIELD         ),
				json::pair(&Activity::assets       , "assets"        , json::OPTIONAL_FIELD         ),
				json::pair(&Activity::secrets      , "secrets"       , json::OPTIONAL_FIELD         ),
				json::pair(&Activity::instance     , "instance"      , json::OPTIONAL_FIELD         ),
				json::pair(&Activity::flags        , "flags"         , json::OPTIONAL_FIELD         )
			);
		JSONStructEnd
	};

	struct PresenceUpdate : public DiscordObject {
	public:
		PresenceUpdate() = default;
		~PresenceUpdate() = default;
		PresenceUpdate(const json::Value & json);
		PresenceUpdate(const nonstd::string_view & json);
		User user;
		std::vector<Snowflake<Role>> roleIDs;
		Activity currentActivity;
		Snowflake<Server> serverID;
		std::string status;
		std::vector<Activity> activities;

		JSONStructStart
			std::make_tuple(
				json::pair(&PresenceUpdate::user           , "user"      , json::REQUIRIED_FIELD),
				json::pair(&PresenceUpdate::roleIDs        , "roles"     , json::REQUIRIED_FIELD),
				json::pair(&PresenceUpdate::currentActivity, "game"      , json::NULLABLE_FIELD ),
				json::pair(&PresenceUpdate::serverID       , "guild_id"  , json::REQUIRIED_FIELD),
				json::pair(&PresenceUpdate::status         , "status"    , json::REQUIRIED_FIELD),
				json::pair(&PresenceUpdate::activities     , "activities", json::REQUIRIED_FIELD)
			);
		JSONStructEnd
	};
}