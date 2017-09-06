﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace HearthstoneAI.Parsers
{
    class ParserUtilities
    {
        public static readonly Regex EntityRegex =
            new Regex(@"(?=id=(?<id>(\d+)))(?=name=(?<name>(\w+)))?(?=zone=(?<zone>(\w+)))?(?=zonePos=(?<zonePos>(\d+)))?(?=cardId=(?<cardId>(\w+)))?(?=player=(?<player>(\d+)))?(?=type=(?<type>(\w+)))?");

        static public int GetEntityIdFromRawString(GameState game_state, string entity_raw)
        {
            entity_raw = entity_raw.Replace("UNKNOWN ENTITY ", "");
            int entityId = -1;

            // Get entity id - Method 1
            if (entity_raw.StartsWith("[") && EntityRegex.IsMatch(entity_raw))
            {
                entityId = int.Parse(EntityRegex.Match(entity_raw).Groups["id"].Value);
            }

            // Get entity id - Method 2
            if (entityId < 0)
            {
                if (!int.TryParse(entity_raw, out entityId)) entityId = -1;
            }

            // Get entity id - Method 3
            if (entityId < 0)
            {
                var entity = game_state.Entities.FirstOrDefault(x => x.Value.Name == entity_raw);

                if (entity.Value == null)
                {
                    // TODO: check when this happens?
                    entity = game_state.Entities.FirstOrDefault(x => x.Value.Name == "UNKNOWN HUMAN PLAYER");
                    if (entity.Value != null) entity.Value.Name = entity_raw;
                }

                if (entity.Value != null) entityId = entity.Key;
                else entityId = -1;
            }

            return entityId;
        }

        static public IEnumerable<bool> TrySubParsers(List<Func<IEnumerable<bool>>> parsers)
        {
            foreach (var parser in parsers)
            {
                bool matched = false;
                foreach (var ret in parser())
                {
                    matched = true;
                    yield return ret;
                }
                
                if (matched) yield break; // match the first parser
            }
        }
    }
}
